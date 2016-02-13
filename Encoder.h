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
	const string encoderFrequencyTableLogName = "C:\\Users\\Max\\Desktop\\Encoder\\encoderFrqTbl.txt";
	const string huffTreeLogFileName = "C:\\Users\\Max\\Desktop\\Encoder\\encoderHuffTree.txt";
	HuffmanTree tree;

	bool createFrequencyTableFromFile(ifstream &f, FrequencyTable &frequencyTable);
	void encodeFile(const FrequencyTable &frequencyTable, ifstream &f, const string &fName);
	FrequencyTable parseIntoFrequencyTable(ifstream &f);
	void writeCompressedCharsToFile(const FrequencyTable &frequencyTable, ostream &encoded, ifstream &original);
	void writeFrequencyTableToFile(const FrequencyTable &frequencyTable, ostream &encoded) const;
	void writeNumberOfUncompressedCharsToFile(const FrequencyTable &frequencyTable, ofstream &encoded) const;
};

