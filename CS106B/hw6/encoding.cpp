// Gregory McInnes
// CS106B
// HW6
// This program compressed and decompresses files using Huffman Encoding
// Files to be decompressed must have a header that contains the integer value
// of the ascii code for each character in the file and the number of times
// it appears, one per line, with the number of unique characters in the first line.

#include "encoding.h"
#include "istream"
#include "bitstream.h"
#include "pqueue.h"
#include "HuffmanNode.h"
#include "huffmanutil.h"
#include "strlib.h"
#include "filelib.h"
#include <string>

// Declarations
void writeFileHeader(ostream& output, Map<int, int>& freqTable);
Map<int, int> readFileHeader(istream& infile);
void constructTree(PriorityQueue<HuffmanNode*> & pq);
void buildEncodingMapHelper(HuffmanNode*& encodingTree, Map<int, string> & encodingMap, string binary);
void decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output);
PriorityQueue<HuffmanNode*> freqTableToPQ(const Map<int, int>& freqTable);

// Build a frequency table that counts the number of times each character appears.
Map<int, int> buildFrequencyTable(istream& input) {
    int byte = input.get();
    Map<int, int> freqTable;
    while (byte) {
        if (byte == -1) {
            byte = 256;
        }
        freqTable[byte]++;
        if (byte == 256) {
            break;
        }
        byte = input.get();
    }
    return freqTable;
}

// Create a priority queue for each character in the frequency table
PriorityQueue<HuffmanNode*> freqTableToPQ(const Map<int, int>& freqTable) {
    if (freqTable.size() == 0) {
        throw "Empty frequency table!";
    }
    PriorityQueue<HuffmanNode*> pq;
    for (int key : freqTable) {
        HuffmanNode* node = new HuffmanNode;
        node->count = freqTable[key];
        node->character = key;
        node->zero = NULL;
        node->one = NULL;
        pq.add(node, double(freqTable[key]));

    }
    return pq;
}

// Build the encoding tree based on the priority queue
void constructTree(PriorityQueue<HuffmanNode*> & pq) {
    if (pq.size() < 2) {
        return;
    } else {
        HuffmanNode* zero = pq.dequeue();
        HuffmanNode* one = pq.dequeue();
        int sum = zero->count + one->count;
        HuffmanNode* newNode = new HuffmanNode;
        newNode->count = sum;
        newNode->zero = zero;
        newNode->one = one;
        pq.enqueue(newNode, double(sum));
        constructTree(pq);
    }
}

// Main encoding tree function
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq = freqTableToPQ(freqTable);
    constructTree(pq);
    return pq.dequeue();
}

// Helper function to search through the encoding tree recusively
void buildEncodingMapHelper(HuffmanNode*& encodingTree, Map<int, string> & encodingMap, string binary) {
    if(encodingTree->isLeaf()) {
        encodingMap[encodingTree->character] = binary;
    } else {
        buildEncodingMapHelper(encodingTree->zero, encodingMap, binary + "0");
        buildEncodingMapHelper(encodingTree->one, encodingMap, binary + "1");
    }
}

// Main encoding map function
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    buildEncodingMapHelper(encodingTree, encodingMap, "");
    return encodingMap;
}

// Convert input to binary based on a custom encoding map
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while(true) {
        int byte = input.get();
        if (byte == -1) {
            byte = 256;
        }
        if (encodingMap.containsKey(byte)) {
            string binary = encodingMap[byte];
            for (int i = 0; i < binary.length(); i++) {

                int bit = stringToInteger(string() + binary[i]);
                output.writeBit(bit);
            }

        } else {
            throw "Value not found in map!";
        }
        if (byte == 256) {
            break;
        }
    }
}

// Helper function to decode data recursively
void decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    if (encodingTree->isLeaf()) {
        output.put(encodingTree->character);
        return;
    } else {
        int bit = input.readBit();
        if (bit == 0) {
            decodeDataHelper(input, encodingTree->zero, output);
        } else if (bit == 1) {
            decodeDataHelper(input, encodingTree->one, output);
        }
    }
}

// Main decoder function
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while (true) {
        int bit = input.readBit();
        if (bit == -1) {
            break;
        }
        if (bit == 0) {
            decodeDataHelper(input, encodingTree->zero, output);
        } else if (bit == 1) {
            decodeDataHelper(input, encodingTree->one, output);
        }
    }
}

// Compress input by creating a custom map.  Output compressed data into a new file
// with the map in the header
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* head = buildEncodingTree(freqTable);
    Map<int, string> map = buildEncodingMap(head);
    writeFileHeader(output, freqTable);
    rewindStream(input);
    encodeData(input, map, output);
    freeTree(head);
}

// Decompress data from a file by reading in the map from the header
// and using that map to recreate the contents
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable = readFileHeader(input);
    HuffmanNode* head = buildEncodingTree(freqTable);
    decodeData(input, head, output);
    freeTree(head);
}

// Clear the contents of a tree
void freeTree(HuffmanNode* node) {
    if(node == NULL) return;
    freeTree(node->zero);
    freeTree(node->one);
    delete node;
}

// Write the header
void writeFileHeader(ostream& output, Map<int, int>& freqTable) {
    output << freqTable.size() - 1 << endl;
    for (int key : freqTable) {
        output << key << " " << freqTable[key] << endl;
    }
}

// Read the header
Map<int, int> readFileHeader(istream& infile) {
    Map<int, int> header;
    string count;
    getline(infile, count);
    int numValues = stringToInteger(count);
    for (int i = 0; i <= numValues; i++) {
        string line;
        getline(infile, line);
        Vector<string> values = stringSplit(line, " ");
        int key = stringToInteger(values[0]);
        header[key] = stringToInteger(values[1]);

    }
    return header;
}

