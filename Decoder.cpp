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
	this->logFile = new ofstream(this->bitsLogFileName, ios::out | ios::binary);
	this->hexLog = new ofstream(this->hexLogFileName);
}


Decoder::~Decoder()
{
	this->logFile->close();
	delete this->logFile;

	this->hexLog->close();
	delete this->hexLog;
}

void Decoder::Decode(ifstream &f, const string & fName)
{
	ParsedEncodedFile parsedFile;
	bool fileValid = parsedFile.Parse(f, fName);
	if (fileValid)
	{
		decodeInputFile(parsedFile, f);
	}
	else
	{
		cout << "File is invalid or missing." << endl;
		return;
	}
}



uint8_t Decoder::getLargestNumberOfBitsToEncode(const Encoding & encoding) const
{
	uint8_t largestSoFar = 0;
	for (unsigned int i = 0; i < 256; i++)
	{
		uint8_t numBits = encoding.encoding[i].numberOfBits;
		largestSoFar = (numBits > largestSoFar) ? numBits : largestSoFar;
	}
	return largestSoFar;
}

uint8_t Decoder::getSmallestNumberOfBitsToEncode(const Encoding & encoding) const
{
	uint8_t smallestSoFar = 255;
	for (unsigned int i = 0; i < 256; i++)
	{
		uint8_t numBits = encoding.encoding[i].numberOfBits;
		if ((numBits != 0) && (numBits < smallestSoFar))
			smallestSoFar = numBits;
	}
	return smallestSoFar;
}

void Decoder::decodeInputFile(ParsedEncodedFile &parsedFile, ifstream &encoded)
{
	HuffmanTree huff(parsedFile.getFrequencyTable());
	huff.Log(huffTreeLogFileName);
	Encoding enc = huff.getEncoding(false);
	string name = parsedFile.getFileName() + ".puff";
	ofstream decoded(name);
	decompressCharacters(huff, encoded, parsedFile.getNumberOfUncompressedChars(), decoded);
	decoded.close();
}

void Decoder::decompressCharacters(HuffmanTree &tree, ifstream &encoded, uint32_t numberOfUncompressedChars, ofstream &decoded)
{
	uint64_t pos = encoded.tellg();
	encoded.seekg(0, encoded.end);
	uint64_t end = encoded.tellg();
	uint64_t fileLength = end - pos;
	encoded.seekg(pos);

	vector<uint8_t> bits;
	vector<uint8_t> bitsToCheck;
	unsigned int i = 0;
	uint64_t numberDecodedSoFar = 0;
	uint8_t largestNumberOfBitsToEncodeACharacter = getLargestNumberOfBitsToEncode(tree.getEncoding(false));
	uint8_t smallestNumberOfBitsToEncodeACharacter = getSmallestNumberOfBitsToEncode(tree.getEncoding(false));




	while (numberDecodedSoFar != numberOfUncompressedChars)
	{//for each byte in the file

		/*
		Collect the next 8 bits from the file and add them on to the bits buffer
		*/
		if ((pos != end) && (bits.size() < largestNumberOfBitsToEncodeACharacter))
		{
			char *nextByte = new char;
			encoded.read(nextByte, 1);
			pos++;
			vector<uint8_t> nextBytesBits = parseCharToBits(*nextByte);
			for (unsigned int j = 0; j < 8; j++)
				bits.push_back(nextBytesBits.at(j));


			uint8_t logByte = *nextByte;
			*this->logFile << logByte;
			*this->hexLog << (this->lineCount)++ << ". 0x" << hex << int(logByte) << endl;
			delete nextByte;
			nextByte = nullptr;
		}

		bitsToCheck.push_back(bits.at(i++));

		uint8_t asciiChar = 0;

		if (bitsToCheck.size() < smallestNumberOfBitsToEncodeACharacter)
			continue;

		bool successfullyDecoded = tree.Decode(bitsToCheck, &asciiChar);
		if (successfullyDecoded)
		{
			//Erase however many bits that was from the front of the bit buffer, to free up space in memory as we go
			bits.erase(bits.begin(), bits.begin() + bitsToCheck.size());
			i = 0;
			bitsToCheck.clear();
			numberDecodedSoFar++;
			decoded << asciiChar;
		}
	}
}

vector<uint8_t> Decoder::parseCharToBits(char c) const
{
	vector<uint8_t> bits;
	for (int i = 7; i >= 0; i--)
		bits.push_back((c >> i) & 0x01);
	return bits;
}