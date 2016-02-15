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

	string getFileName() const;
	FrequencyTable getFrequencyTable() const;
	uint32_t getNumberOfUncompressedChars() const;

	bool Parse(ifstream &encoded, const string &fName);

private:
	FrequencyTable frequencyTable;
	string fileName;
	uint32_t numberOfUncompressedChars = 0;
#if defined ENABLE_LOGS
	const string decoderFrequencyTableLogName = "C:\\Users\\Max\\Desktop\\Encoder\\decoderFrqTbl.txt";
#endif
	FrequencyTable createFrequencyTable(const vector<string> &frequenciesAsStrings) const;
	void parseFrequencyTable(ifstream &encoded, unsigned int &j);
	void parseNumberOfChars(ifstream &encoded, unsigned int &j);
	void parseTableFromFile(ifstream &encoded, unsigned int &j);
};