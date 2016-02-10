#include "stdafx.h"
#include "ParsedEncodedFile.h"
#include <fstream>
#include <iostream>
#include <string>
#include "FVPair.h"
#include <string>
#include "FrequencyTable.h"

using namespace std;

ParsedEncodedFile::ParsedEncodedFile()
{
}

ParsedEncodedFile::~ParsedEncodedFile()
{
}

vector<uint8_t> ParsedEncodedFile::getBits() const
{
	return this->bits;
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
	vector<char> allLines(istreambuf_iterator<char>(encoded), (istreambuf_iterator<char>()));

	unsigned int j = 0;
	parseFrequencyTable(allLines, j);
	parseNumberOfChars(allLines, j);
	parseBits(allLines, j);
	
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

void ParsedEncodedFile::parseBits(vector<char>& wholeFile, unsigned int & j)
{
	vector<uint8_t> bytes;
	for (; j < wholeFile.size(); j++)
	{
		bytes.push_back(wholeFile.at(j));
	}

	//convert the bytes into a vector of "bits" where each byte is a 0 or 1 from msb to lsb
	for (unsigned int i = 0; i < bytes.size(); i++)
	{
		uint8_t byte = bytes.at(i);

		for (int k = 7; k >= 0; k--)
		{
			uint8_t bit = (byte >> k) & 0x01;
			this->bits.push_back(bit);
		}
	}
}

void ParsedEncodedFile::parseFrequencyTable(vector<char> &wholeFile, unsigned int &j)
{
	//parse the table out of the file
	vector<string> allFrequencies = parseTableFromFile(wholeFile, j);

	//set up the frequency table object
	this->frequencyTable = createFrequencyTable(allFrequencies);
}

void ParsedEncodedFile::parseNumberOfChars(vector<char> &wholeFile, unsigned int &j)
{
	uint8_t numberOfDigitsInUncompressedChars = wholeFile.at(j);
	unsigned int start = j;
	j++;
	string numberOfUncompressedChars;
	for (; j <= start + numberOfDigitsInUncompressedChars; j++)
		numberOfUncompressedChars += wholeFile.at(j);

	this->numberOfUncompressedChars = stoi(numberOfUncompressedChars);
}

vector<string> ParsedEncodedFile::parseTableFromFile(vector<char>& wholeFile, unsigned int & j)
{
	vector<string> allFrequenciesAsStrings;
	j = 0;
	for (unsigned int i = 0; i < 256; i++)
	{
		uint8_t numberOfDigitsInNextFrequency = wholeFile.at(j);
		unsigned int start = j;
		j++;
		string frequency;
		for (; j <= start + numberOfDigitsInNextFrequency; j++)
		{
			frequency += wholeFile.at(j);
		}
		allFrequenciesAsStrings.push_back(frequency);
	}

	return allFrequenciesAsStrings;
}
