#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <vector>
#include <map>
#include <fstream>

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char ch, int freq, Node* l = nullptr, Node* r = nullptr)
        : ch(ch), freq(freq), left(l), right(r) {}
};

class Huffman {
public:
    void compress(const std::string& inputFile, const std::string& outputFile);
    void decompress(const std::string& inputFile, const std::string& outputFile);

private:
    std::map<char, int> freqMap;
    std::map<char, std::string> codeMap;
    Node* root = nullptr;
    long long totalCharacters = 0; // To track exact file size for decompression

    void calculateFrequencies(const std::string& inputFile);
    void buildTree();
    void generateCodes(Node* node, std::string code);
    void writeHeader(std::ofstream& outFile);
    void readHeader(std::ifstream& inFile);
    void deleteTree(Node* node);
};

#endif
