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
		frequencyTable.Log(encoderFrequencyTableLogName);
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
	ofstream encoded(encodedFileName, ios::out | ios::binary);

	writeFrequencyTableToFile(frequencyTable, encoded);
	writeNumberOfUncompressedCharsToFile(frequencyTable, encoded);
	writeCompressedCharsToFile(frequencyTable, encoded, f);
	encoded.close();
}

FrequencyTable Encoder::parseIntoFrequencyTable(ifstream &f)
{
	/*
	Create an array of ascii char frequencies with the array index being the ascii char itself
	*/

	//initialize the array
	uint32_t asciiCharFrequencies[256];
	for (unsigned int i = 0; i < 256; i++)
		asciiCharFrequencies[i] = 0;

	//fill the array
	f.seekg(0, f.end);
	uint64_t fileLength = f.tellg();
	f.seekg(0, f.beg);
	for (unsigned int i = 0; i < fileLength; i++)
	{
		char *nextAsciiChar = new char;
		f.read(nextAsciiChar, 1);
		
		uint8_t c = *nextAsciiChar;
		asciiCharFrequencies[c] += 1;

		delete nextAsciiChar;
		nextAsciiChar = nullptr;
	}

	FrequencyTable table(asciiCharFrequencies);

	return table;
}

void Encoder::writeCompressedCharsToFile(const FrequencyTable &frequencyTable, ostream &encoded, ifstream &original)
{
	uint64_t numChars = 0;

	HuffmanTree huff(frequencyTable);
	huff.Log(huffTreeLogFileName);
	Encoding encoding = huff.getEncoding(true);
	BufferWriter writer;

	original.seekg(0, original.end);
	uint64_t fileLength = original.tellg();
	original.seekg(0, original.beg);
	for (unsigned int i = 0; i < fileLength; i++)
	{
		numChars++;
		char *c = new char;
		original.read(c, 1);
		uint8_t nextAsciiCharFromFile = *c;
		delete c;
		c = nullptr;

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
	uint32_t numberOfUncompressedCharsInFile = frequencyTable.getTotal();
	uint8_t numberOfDigitsInUncompressedCharsInFile = frequencyTable.calculateNumberOfDigits(numberOfUncompressedCharsInFile);

	//char *numDigs = new char;
	//*numDigs = numberOfDigitsInUncompressedCharsInFile;
	//encoded.write(numDigs, 1);
	//delete numDigs;
	//numDigs = nullptr;

	//vector<char> digits = frequencyTable.getDigits(numberOfUncompressedCharsInFile);

	encoded << numberOfDigitsInUncompressedCharsInFile;
	encoded << numberOfUncompressedCharsInFile;
}
