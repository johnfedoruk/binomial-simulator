CC=g++
CFLAGS=-Wall -std=c++11 -pthread -O3
OBJECTS=main.o
TARGET=run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o