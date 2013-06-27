CC=g++
CFLAGS=-g -O3 -std=c++11
LIBS=-lSDL

all:
	$(CC) $(CFLAGS) src/*.cpp -o ibm51501 $(LIBS)

clean:
	rm ibm5150
