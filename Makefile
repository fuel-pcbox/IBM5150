CC=g++
CFLAGS=-std=c++11
LIBS=-lSDL

all:
	$(CC) $(CFLAGS) src/*.cpp -o ibm5150 $(LIBS)
	
debug:
	$(CC) $(CFLAGS) -g -pg src/*.cpp -o ibm5150 $(LIBS) -DDEBUG

clean:
	rm ibm5150
