#pragma once

#include <vector>
#include <fstream>
#include "FVPair.h"
#include "FrequencyTable.h"

using namespace std;

class ParsedEncodedFile
{
public:
	ParsedEncodedFile();
	~ParsedEncodedFile();

	vector<uint8_t> getBits() const;
	string getFileName() const;
	FrequencyTable getFrequencyTable() const;
	uint32_t getNumberOfUncompressedChars() const;

	bool Parse(ifstream &encoded, const string &fName);

private:
	FrequencyTable frequencyTable;
	string fileName;
	uint32_t numberOfUncompressedChars = 0;
	//bits (each "bit" is a byte that is either a 0 or a 1)
	vector<uint8_t> bits;

	FrequencyTable createFrequencyTable(const vector<string> &frequenciesAsStrings) const;
	void parseBits(vector<char> &wholeFile, unsigned int &j);
	void parseFrequencyTable(vector<char> &wholeFile, unsigned int &j);
	void parseNumberOfChars(vector<char> &wholeFile, unsigned int &j);
	vector<string> parseTableFromFile(vector<char> &wholeFile, unsigned int &j);
};