#include "stdafx.h"
#include "Decoder.h"
#include <string>
#include <vector>
#include <fstream>
#include "HuffmanTree.h"
#include "ParsedEncodedFile.h"
#include "PriorityQueue.h"
#include "FVPair.h"

using namespace std;

Decoder::Decoder()
{
}


Decoder::~Decoder()
{
}

void Decoder::Decode(ifstream &f, const string & fName) const
{
	ParsedEncodedFile parsedFile;
	bool fileValid = parsedFile.Parse(f, fName);
	if (fileValid)
	{
		decodeInputFile(parsedFile);
	}
	else
	{
		cout << "File is invalid or missing." << endl;
		return;
	}
}



void Decoder::decodeInputFile(ParsedEncodedFile &parsedFile) const
{
	HuffmanTree huff(parsedFile.getFrequencyTable());
	Encoding enc = huff.getEncoding(false);
	string name = parsedFile.getFileName() + ".puff";
	ofstream decoded(name);
	vector<uint8_t> uncompressedAsciiChars = decompressCharacters(huff, parsedFile);
	writeToFile(uncompressedAsciiChars, decoded);
}

vector<uint8_t> Decoder::decompressCharacters(const HuffmanTree & tree, const ParsedEncodedFile & encoded) const
{
	vector<uint8_t> bits = encoded.getBits();
	vector<uint8_t> bitsToCheck;
	vector<uint8_t> asciiChars;
	unsigned int i = 0;
	while (i < bits.size())
	{//for each bit in bits, check if it reaches a character
		bitsToCheck.push_back(bits.at(i));

		uint8_t asciiChar = 0;
		bool decoded = tree.Decode(bitsToCheck, &asciiChar);				
		if (decoded)
		{
			bitsToCheck.clear();
			asciiChars.push_back(asciiChar);
		}

		i++;
		if (asciiChars.size() >= encoded.getNumberOfUncompressedChars())
			break;
	}

	return asciiChars;
}

void Decoder::writeToFile(const vector<uint8_t>& uncompressedAsciiChars, ofstream & f) const
{
	for (unsigned int i = 0; i < uncompressedAsciiChars.size(); i++)
		f << uncompressedAsciiChars.at(i);
	
	f.close();
}
