# 🗜 Huffman Comparator

A C++ command-line tool for compressing and decompressing files using Huffman coding.

## ▶ Features
- Compress any file (text, images, binary) into optimized `.huf` format.
- Decompress `.huf` files back to their original state.
- Visual progress indicators.

## 🛠️ Building via Command Line

### Prerequisites
- C++ Compiler (GCC, Clang, or MSVC) supporting C++11 or later.
- Make (optional).

### Compilation
Using Make:
```bash
make
```

Using G++ directly:
```bash
g++ -std=c++11 main.cpp Huffman.cpp -o huffman.exe
```

## Usage

### Compression
```bash
./huffman -c <input_file> <output_file>
```
Example:
```bash
./huffman -c document.txt document.huf
```

### Decompression
```bash
./huffman -d <input_file> <output_file>
```
Example:
```bash
./huffman -d document.huf recovered.txt
```

## 🗁  File Structure
```
Huffman-Compressor/
├── Huffman.h /* Header file for Huffman class and Node structure.
├── Huffman.cpp // Implementation of compression algorithms.
├── main.cpp // Entry point and CLI handling.
└── Makefile // Build script.