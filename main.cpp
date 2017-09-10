#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>
#include <boost/format.hpp>
#include <thread>
#include <vector>
#include <mutex>
#include "main.hpp"

#define VERBOSE false

// functions

int main(int argc,char* argv[]) {
    int sample_success_count = 0;
    // calculate the allocated sample size for each thread
	static int SAMPLES_PER_THREAD = SAMPLE_SIZE / THREAD_COUNT;
	// verbose output showing config
	if(VERBOSE)
		std::cout << "SAMPLES_PER_THREAD: " << SAMPLES_PER_THREAD << "\tSAMPLE_SIZE: " << SAMPLE_SIZE << "\tTHREAD_COUNT: " << THREAD_COUNT << std::endl;
    std::vector<std::thread> threads;   // a vector to hold threads
    // create each thread and push into vector
    for(int i=0;i<THREAD_COUNT;i++) 
        threads.push_back(std::thread(worker,i,SAMPLES_PER_THREAD,&sample_success_count));
    // block and wait for each thread to join
    for(int i=0;i<THREAD_COUNT;i++)
		threads[i].join();
    // print the results
    std::cout<<std::fixed<<"~"<<experimentalProb(sample_success_count)<<"% probability of having "<<getTestStr()
    <<" "<<TARGET_HEAD_COUNT<<" heads in "<<ITERATIONS_PER_SAMPLE<<" flips"<<std::endl;
    // return 1.. just kidding.
    return 0;
}
void worker(int id,const int SAMPLES_PER_THREAD,int* sample_success_count) {
	// log that thread id started if verbose
	if(VERBOSE)
		print((boost::format("thread %02d started") % id ).str());
	// create a randomizer function to pass to flipCoin
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(0,1);
	std::function<int()> rand_func = std::bind(distribution,generator);
	// instantiate head/tail count
	int heads_in_sample;
	int tails_in_sample;
	// do SAMPLE_SIZE tests
	for(int i=0;i<SAMPLES_PER_THREAD;i++) {
		heads_in_sample = 0;
		tails_in_sample = 0;
		// perform a test with ITERATIONS_PER_SAMPLE
		// coin flips
		for(int j=0;j<ITERATIONS_PER_SAMPLE;j++) {
			Coin coin = flipCoin(&rand_func);
			if(coin==heads)
				heads_in_sample++;
			else
				tails_in_sample++;
		}
		// count the successes
		if(sampleSuccess(heads_in_sample)) 
			updateSampleSuccessCount(sample_success_count);
	}
	// log that thread id started if verbose
	if(VERBOSE)
		print((boost::format("thread %02d finished") % id ).str());
}
void updateSampleSuccessCount(int* sample_success_count) {
	std::lock_guard<std::mutex> guard(ssc_m);
	(*sample_success_count)++;
}
Coin flipCoin(std::function<int()>* rand_func) {
	return (*rand_func)()?heads:tails;
}
bool atleastN(int k,int n) {
    return k>=n;
}
bool exactlyN(int k,int n) {
    return k==n;
}
bool sampleSuccess(int heads_in_sample) {
    bool (*test_func)(int,int);
    switch(COIN_TEST) {
        case exactly:
            test_func = exactlyN;
            break;
        case atleast:
            test_func = atleastN;
            break;
        default:
            throw(1);
    }
    return test_func(heads_in_sample,TARGET_HEAD_COUNT);
}
double experimentalProb(int success_count) {
    return success_count*100.0/SAMPLE_SIZE;
}
std::string getTestStr() {
    switch(COIN_TEST) {
        case exactly: return "exactly";
        case atleast: return "atleast";
        default: throw(1);
    }
}
void print(std::string message) {
	std::lock_guard<std::mutex> guard(print_m);
	std::cout << message << std::endl;
}