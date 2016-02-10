#include "stdafx.h"
#include <iostream>

#include "HuffmanTree.h"
#include "BufferWriter.h"
#include "Encoding.h"
#include "FVPair.h"

#include "Encoder.h"


using namespace std;

Encoder::Encoder()
{
}


Encoder::~Encoder()
{
}

HuffmanTree Encoder::getTree() const
{
	return this->tree;
}

void Encoder::Encode(ifstream &f, const string &fName)
{
	FrequencyTable frequencyTable;
	bool fileValid = createFrequencyTableFromFile(f, frequencyTable);
	if (fileValid)
	{
		encodeFile(frequencyTable, f, fName);
	}
	else
	{
		cout << "File is missing or invalid." << endl;
		return;
	}
}




bool Encoder::createFrequencyTableFromFile(ifstream & f, FrequencyTable& frequencyTable)
{
	try
	{
		frequencyTable = parseIntoFrequencyTable(f);
		return true;
	}
	catch (exception ex)
	{
		return false;
	}
}

void Encoder::encodeFile(const FrequencyTable &frequencyTable, ifstream & f, const string & fName)
{
	string encodedFileName = fName + ".huf";
	ofstream encoded(encodedFileName);

	writeFrequencyTableToFile(frequencyTable, encoded);
	writeNumberOfUncompressedCharsToFile(frequencyTable, encoded);
	writeCompressedCharsToFile(frequencyTable, encoded);
	encoded.close();
}

void Encoder::getAllCharsFromFile(ifstream & f)
{
	vector<char> allCharsFromFile(istreambuf_iterator<char>(f), (istreambuf_iterator<char>()));

	vector<uint8_t> chars;
	for (unsigned int i = 0; i < allCharsFromFile.size(); i++)
		chars.push_back(allCharsFromFile.at(i));

	this->inputFileAsAsciiChars = chars;
}

FrequencyTable Encoder::parseIntoFrequencyTable(ifstream &f)
{
	getAllCharsFromFile(f);

	/*
	Create an array of ascii char frequencies with the array index being the ascii char itself
	*/

	//initialize the array
	uint32_t asciiCharFrequencies[256];
	for (unsigned int i = 0; i < 256; i++)
		asciiCharFrequencies[i] = 0;

	//fill the array
	for (unsigned int i = 0; i < this->inputFileAsAsciiChars.size(); i++)
	{
		uint8_t c = this->inputFileAsAsciiChars.at(i);
		asciiCharFrequencies[c] += 1;
	}

	FrequencyTable table(asciiCharFrequencies);

	return table;
}

void Encoder::writeCompressedCharsToFile(const FrequencyTable &frequencyTable, ostream & encoded)
{
	HuffmanTree huff(frequencyTable);
	Encoding encoding = huff.getEncoding(true);
	BufferWriter writer;
	for (unsigned int i = 0; i < this->inputFileAsAsciiChars.size(); i++)
	{
		uint8_t nextAsciiCharFromFile = this->inputFileAsAsciiChars.at(i);
		uint8_t numberOfBitsToWrite = encoding.encoding[nextAsciiCharFromFile].numberOfBits;
		uint32_t bitsToWrite = encoding.encoding[nextAsciiCharFromFile].bits;
		writer.Write(numberOfBitsToWrite, bitsToWrite, encoded);		
	}

	writer.FlushBufferToFile(encoded);
}

void Encoder::writeFrequencyTableToFile(const FrequencyTable &frequencyTable, ostream & outStream) const
{
	for (unsigned int i = 0; i < 256; i++)
	{
		uint8_t asciiChar = i;
		uint16_t frequency = frequencyTable.getFrequency(asciiChar);
		uint8_t numberOfDigitsInFrequency = frequencyTable.calculateNumberOfDigits(frequency);
		
		outStream << numberOfDigitsInFrequency;
		outStream << frequency;//converts to ascii digits when printing - so 32 turns into '3''2' in the file
	}
}

void Encoder::writeNumberOfUncompressedCharsToFile(const FrequencyTable &frequencyTable, ofstream & encoded) const
{
	uint32_t numberOfUncompressedCharsInFile = this->inputFileAsAsciiChars.size();
	uint8_t numberOfDigitsInUncompressedCharsInFile = frequencyTable.calculateNumberOfDigits(numberOfUncompressedCharsInFile);

	encoded << numberOfDigitsInUncompressedCharsInFile;
	encoded << numberOfUncompressedCharsInFile;
}
