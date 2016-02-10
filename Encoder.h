#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "PriorityQueue.h"
#include "FVPair.h"
#include "HuffmanTree.h"
#include "FrequencyTable.h"

using namespace std;

class Encoder
{
public:
	Encoder();
	~Encoder();

	HuffmanTree getTree() const;

	void Encode(ifstream &f, const string &fName);


private:
	vector<uint8_t> inputFileAsAsciiChars;
	HuffmanTree tree;

	bool createFrequencyTableFromFile(ifstream &f, FrequencyTable &frequencyTable);
	void encodeFile(const FrequencyTable &frequencyTable, ifstream &f, const string &fName);
	void getAllCharsFromFile(ifstream &f);
	FrequencyTable parseIntoFrequencyTable(ifstream &f);
	void writeCompressedCharsToFile(const FrequencyTable &frequencyTable, ostream &encoded);
	void writeFrequencyTableToFile(const FrequencyTable &frequencyTable, ostream &encoded) const;
	void writeNumberOfUncompressedCharsToFile(const FrequencyTable &frequencyTable, ofstream &encoded) const;
};

