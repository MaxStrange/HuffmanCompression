#include "stdafx.h"
#include <iostream>

#include "HuffmanTree.h"
#include "BufferWriter.h"
#include "Encoding.h"
#include "FVPair.h"

#include "Encoder.h"
#include "CStopWatch.h"


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
		encodeFile(frequencyTable, f, fName, frequencyTable.getTotal());
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
#if defined ENABLE_LOGS
		frequencyTable.Log(encoderFrequencyTableLogName);
#endif
		return true;
	}
	catch (exception ex)
	{
		return false;
	}
}

void Encoder::encodeFile(const FrequencyTable &frequencyTable, ifstream & f, const string & fName, uint32_t numberOfCharsInFile)
{
	string encodedFileName = fName + ".huf";
	ofstream encoded(encodedFileName, ios::out | ios::binary);

	writeFrequencyTableToFile(frequencyTable, encoded);
	writeNumberOfUncompressedCharsToFile(numberOfCharsInFile, encoded);
	writeCompressedCharsToFile(frequencyTable, encoded, f);
	encoded.close();
}

FrequencyTable Encoder::parseIntoFrequencyTable(ifstream &f)
{
	uint32_t total = 0;

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
	uint8_t nextAsciiChar;
	for (unsigned int i = 0; i < fileLength; i++)
	{
		nextAsciiChar = i;
		f.read((char *)&nextAsciiChar, 1);
		total++;
		asciiCharFrequencies[nextAsciiChar] += 1;
	}

	FrequencyTable table(asciiCharFrequencies);
	table.setTotal(total);

	return table;
}

void Encoder::writeCompressedCharsToFile(const FrequencyTable &frequencyTable, ostream &encoded, ifstream &original)
{
	uint64_t numChars = 0;

	HuffmanTree huff(frequencyTable);
#if defined ENABLE_LOGS
	huff.Log(huffTreeLogFileName);
#endif
	Encoding encoding = huff.getEncoding(true);
	BufferWriter writer;

	original.seekg(0, original.end);
	uint64_t fileLength = original.tellg();
	original.seekg(0, original.beg);
	

	char c;

	double totalAllButWriterDuration = 0;
	double totalWriterDuration = 0;
	for (unsigned int i = 0; i < fileLength; i++)
	{
		numChars++;
		original.read(&c, 1);
		uint8_t nextAsciiCharFromFile = c;

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
		
		uint8_t frqUpperByte = (frequency & 0xFF00) >> 8;
		uint8_t frqLowerByte = frequency & 0x00FF;

		outStream.write((char *)&frqUpperByte, 1);
		outStream.write((char *)&frqLowerByte, 1);

	}
}

void Encoder::writeNumberOfUncompressedCharsToFile(uint32_t numberOfUncompressedCharsInFile, ofstream & encoded) const
{
	uint8_t numMSB = ((0xFF000000 & numberOfUncompressedCharsInFile) >> 24);
	uint8_t numByte1 = ((0x00FF0000 & numberOfUncompressedCharsInFile) >> 16);
	uint8_t numByte2 = ((0x0000FF00 & numberOfUncompressedCharsInFile) >> 8);
	uint8_t numLSB = (0x000000FF & numberOfUncompressedCharsInFile);

	encoded.write((char *)&numMSB, 1);
	encoded.write((char *)&numByte1, 1);
	encoded.write((char *)&numByte2, 1);
	encoded.write((char *)&numLSB, 1);
}
