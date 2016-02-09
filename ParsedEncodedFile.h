#pragma once

#include <vector>
#include <fstream>
#include "FVPair.h"

using namespace std;

class ParsedEncodedFile
{
public:
	ParsedEncodedFile();
	~ParsedEncodedFile();

	vector<uint8_t> getBits() const;
	vector<FVPair> getFrequencyTable() const;
	uint32_t getNumberOfUncompressedChars() const;

	bool Parse(ifstream &encoded);

private:
	uint16_t asciiFrequencies[256];
	uint32_t numberOfUncompressedChars = 0;
	//bits (each "bit" is a byte that is either a 0 or a 1)
	vector<uint8_t> bits;
};