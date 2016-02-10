#pragma once

#include <vector>
#include <fstream>
#include "HuffmanTree.h"
#include "ParsedEncodedFile.h"
#include "PriorityQueue.h"

using namespace std;

class Decoder
{
public:
	Decoder();
	~Decoder();

	void Decode(ifstream &f, const string &fName) const;


private:
	void decodeInputFile(ParsedEncodedFile &parsedFile) const;
	vector<uint8_t> decompressCharacters(const HuffmanTree &tree, const ParsedEncodedFile &encoded) const;
	void writeToFile(const vector<uint8_t> &uncompressedAsciiChars, ofstream &f) const;
};

