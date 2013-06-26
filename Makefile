all:
	g++ -g src/main.cpp -o ibm5150 -std=c++11 -lSDL

clean:
	rm ibm5150
