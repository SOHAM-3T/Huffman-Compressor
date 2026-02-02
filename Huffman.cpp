#include "Huffman.h"
#include <queue>
#include <fstream>
#include <iostream>

using namespace std;

// Comparator for priority queue (Min Heap)
struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

void Huffman::calculateFrequencies(const string& inputFile) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open input file: " << inputFile << endl;
        return;
    }

    freqMap.clear();
    totalCharacters = 0;
    char ch;
    while (inFile.get(ch)) {
        freqMap[ch]++;
        totalCharacters++;
    }
    inFile.close();
}

void Huffman::buildTree() {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto const& pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Handle edge case: single character in file
    if (pq.empty()) return;
    if (pq.size() == 1) {
        // Special case creates a dummy parent if only one unique char exists
        Node* child = pq.top(); pq.pop();
        root = new Node('\0', child->freq, child, nullptr);
        // Note: For pure Huffman, usually we need >= 2 nodes to traverse. 
        // If single node, code will be "0" or "1".
        // My generateCodes logic expects left/right traversal.
        return;
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* parent = new Node('\0', left->freq + right->freq, left, right);
        pq.push(parent);
    }

    root = pq.top();
}

void Huffman::generateCodes(Node* node, string code) {
    if (!node) return;

    if (!node->left && !node->right) {
        codeMap[node->ch] = code;
        return;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

void Huffman::writeHeader(ofstream& outFile) {
    // 1. Write map size (int)
    // 2. Write map contents (char, int) pairs
    // 3. Write total characters (long long)
    
    int mapSize = freqMap.size();
    outFile.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    for (auto const& pair : freqMap) {
        char ch = pair.first;
        int freq = pair.second;
        outFile.write(&ch, sizeof(ch));
        outFile.write(reinterpret_cast<char*>(&freq), sizeof(freq));
    }

    outFile.write(reinterpret_cast<char*>(&totalCharacters), sizeof(totalCharacters));
}

void Huffman::readHeader(ifstream& inFile) {
    freqMap.clear();
    int mapSize;
    inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    for (int i = 0; i < mapSize; ++i) {
        char ch;
        int freq;
        inFile.read(&ch, sizeof(ch));
        inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        freqMap[ch] = freq;
    }

    inFile.read(reinterpret_cast<char*>(&totalCharacters), sizeof(totalCharacters));
}

void Huffman::compress(const string& inputFile, const string& outputFile) {
    calculateFrequencies(inputFile);
    if (freqMap.empty()) {
        // Create empty output file if input is empty
        ofstream outFile(outputFile, ios::binary);
        return;
    }
    
    buildTree();
    generateCodes(root, "");

    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    writeHeader(outFile);

    char buffer = 0;
    int bitCount = 0;
    char ch;
    while (inFile.get(ch)) {
        string code = codeMap[ch];
        for (char bit : code) {
            if (bit == '1') {
                buffer |= (1 << (7 - bitCount));
            }
            bitCount++;
            if (bitCount == 8) {
                outFile.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    
    if (bitCount > 0) {
        outFile.put(buffer);
    }

    inFile.close();
    outFile.close();
    
    // Cleanup
    deleteTree(root);
    root = nullptr;
}

void Huffman::decompress(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);
    
    // Check if empty file
    inFile.seekg(0, ios::end);
    if (inFile.tellg() == 0) {
        return;
    }
    inFile.seekg(0, ios::beg);

    readHeader(inFile);
    buildTree();

    Node* curr = root;
    long long count = 0;
    char byte;
    
    while (inFile.get(byte)) {
        for (int i = 0; i < 8; ++i) {
            if (count >= totalCharacters) break;

            if (byte & (1 << (7 - i))) {
                curr = curr->right;
            } else {
                curr = curr->left;
            }

            if (!curr->left && !curr->right) {
                outFile.put(curr->ch);
                curr = root;
                count++;
            }
        }
        if (count >= totalCharacters) break;
    }

    inFile.close();
    outFile.close();

    // Cleanup
    deleteTree(root);
    root = nullptr;
}

void Huffman::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}
