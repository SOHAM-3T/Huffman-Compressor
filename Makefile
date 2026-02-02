CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: huffman

huffman: main.o Huffman.o
	$(CXX) $(CXXFLAGS) -o huffman main.o Huffman.o

main.o: main.cpp Huffman.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Huffman.o: Huffman.cpp Huffman.h
	$(CXX) $(CXXFLAGS) -c Huffman.cpp

clean:
	rm -f *.o huffman huffman.exe
