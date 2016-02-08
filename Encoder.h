#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "PriorityQueue.h"
#include "FVPair.h"
#include "HuffmanTree.h"

using namespace std;

class Encoder
{
public:
	Encoder();
	~Encoder();

	void Encode(ifstream &f, const string &fName);


private:
	vector<uint8_t> inputFileAsAsciiChars;

	HuffmanTree buildHuffmanTree(PriorityQueue<FVPair> &pq) const;
	vector<FVPair> parse(ifstream &f);
	vector<uint8_t> parseToAsciiChars(ifstream &f) const;
	void writeFrequencyTableToFile(vector<FVPair> frequencyTable, ostream &outStream) const;
};

