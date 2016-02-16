#include "stdafx.h"

#include <fstream>
#include <iostream>
#include <string>

#include "FVPair.h"
#include "FrequencyTable.h"

#include "ParsedEncodedFile.h"


using namespace std;

ParsedEncodedFile::ParsedEncodedFile()
{
}

ParsedEncodedFile::~ParsedEncodedFile()
{
}

string ParsedEncodedFile::getFileName() const
{
	return this->fileName;
}

FrequencyTable ParsedEncodedFile::getFrequencyTable() const
{
	return this->frequencyTable;
}

uint32_t ParsedEncodedFile::getNumberOfUncompressedChars() const
{
	return this->numberOfUncompressedChars;
}

bool ParsedEncodedFile::Parse(ifstream & encoded, const string &fName)
{
	this->fileName = fName;

	unsigned int j = 0;
	parseTableFromFile(encoded, j);
#if defined ENABLE_LOGS
	this->frequencyTable.Log(decoderFrequencyTableLogName);
#endif
	parseNumberOfChars(encoded, j);
	
	return true;
}

FrequencyTable ParsedEncodedFile::createFrequencyTable(const vector<string>& frequenciesAsStrings) const
{
	FrequencyTable table;

	for (unsigned int i = 0; i < frequenciesAsStrings.size(); i++)
	{
		uint16_t frequency = stoi(frequenciesAsStrings.at(i));

		if (frequency == 0)
			continue;

		uint8_t asciiChar = i;
		FVPair fv;
		fv.frequency = frequency;
		fv.value = asciiChar;
		table.push_back(fv);
	}
	return table;
}

void ParsedEncodedFile::parseNumberOfChars(ifstream &encoded, unsigned int &j)
{
	//just read in the next four bytes and combine them into a uint32_t
	for (int i = 3; i >= 0; i--)
	{
		uint8_t nextByte;
		encoded.read((char *)&nextByte, 1);
		j++;
		uint32_t nextByteAs32Bit = (uint32_t)nextByte;
		this->numberOfUncompressedChars |= (nextByteAs32Bit << (i * 8));
	}
}

void ParsedEncodedFile::parseTableFromFile(ifstream &encoded, unsigned int & j)
{
	j = 0;
	for (int i = 0; i < 256; i++)
	{
		FVPair fv;
		uint8_t value = i;
		uint16_t frequency;

		uint8_t frqMSB;
		encoded.read((char *)&frqMSB, 1);
		j++;
		uint8_t frqLSB;
		encoded.read((char *)&frqLSB, 1);
		j++;

		frequency = ((uint16_t)(frqMSB) << 8) | (uint16_t)(frqLSB);


		if (frequency == 0)
			continue;
		fv.frequency = frequency;
		fv.value = value;
		this->frequencyTable.push_back(fv);
	}
}
