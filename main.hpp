// enums

enum Test { exactly, atleast };
enum Coin { heads, tails };

// constants

const Test COIN_TEST = exactly;         // type of test, see Test emun for options
const int SAMPLE_SIZE = 100000000;		// sample size (number of tests)
const int ITERATIONS_PER_SAMPLE = 30;   // how many flips for every individual sample test
const int TARGET_HEAD_COUNT = 22;       // how many heads we're aiming for in a given sample test
const int THREAD_COUNT = 64;			// number of threads to run

std::mutex ssc_m;						// mutex for updating sample_success_count
std::mutex print_m;						// mutex for printing to console

// prototypes

/**
	worker
    --------------------
	Performs the parallel calc
**/
void worker(int id,const int SAMPLES_PER_THREAD,int* sample_success_count);
/**
	updateSampleSuccessCount
	--------------------
	Increments the sample_success_count by one. Holds a mutex.
**/
void updateSampleSuccessCount(int* sample_success_count);
/**
    flipCoin
    --------------------
    Returns a heads or tails coin, 50% prob of either
**/
Coin flipCoin(std::function<int()>* rand_func);
/**
    atleastN
    --------------------
    Returns true iff k is atleast n
**/
bool atleastN(int k,int n);
/**
    exactlyN
    --------------------
    Returns true iff k is exactly n
**/
bool exactlyN(int k,int n);
/**
    sampleSuccess
    --------------------
    Returns true if the test passes the COIN_TEST
    for a given TARGET_HEAD_COUNT
**/
bool sampleSuccess(int heads_in_sample);
/**
    getRandomFunction
    --------------------
    Returns a boolean randomizer
**/
// auto* getRandomFunction();
/**
    experimentalProb
    --------------------
    Calculates the experimental probability of
    COIN_TEST occuring for a given TARGET_HEAD_COUNT
**/
double experimentalProb(int success_count);
/**
    getTestStr
    --------------------
    Returns the string value for COIN_TEST
**/
std::string getTestStr();
/**
	print
    --------------------
	Allows for parallel printing to console
**/
void print(std::string message);