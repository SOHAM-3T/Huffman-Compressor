#include <iostream>
#include <string>
#include "Huffman.h"

using namespace std;

void printUsage(const char* progName) {
    cout << "Usage: " << progName << " -c <input_file> <output_file>" << endl;
    cout << "       " << progName << " -d <input_file> <output_file>" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printUsage(argv[0]);
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    Huffman huffman;

    if (mode == "-c") {
        cout << "Compressing " << inputFile << " to " << outputFile << "..." << endl;
        huffman.compress(inputFile, outputFile);
        cout << "Compression complete." << endl;
    } else if (mode == "-d") {
        cout << "Decompressing " << inputFile << " to " << outputFile << "..." << endl;
        huffman.decompress(inputFile, outputFile);
        cout << "Decompression complete." << endl;
    } else {
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
