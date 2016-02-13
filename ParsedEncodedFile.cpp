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
	parseFrequencyTable(encoded, j);
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

	table.Log(decoderFrequencyTableLogName);
	return table;
}

void ParsedEncodedFile::parseFrequencyTable(ifstream &encoded, unsigned int &j)
{
	//parse the table out of the file
	vector<string> allFrequencies = parseTableFromFile(encoded, j);

	//set up the frequency table object
	this->frequencyTable = createFrequencyTable(allFrequencies);
}

void ParsedEncodedFile::parseNumberOfChars(ifstream &encoded, unsigned int &j)
{
	char *numberOfDigitsInUncompressedChars = new char;
	encoded.read(numberOfDigitsInUncompressedChars, 1);

	unsigned int start = j;
	j++;
	string numberOfUncompressedChars;
	for (; j <= start + *numberOfDigitsInUncompressedChars; j++)
	{
		char *nextDigit = new char;
		encoded.read(nextDigit, 1);
		numberOfUncompressedChars += *nextDigit;
		delete nextDigit;
		nextDigit = nullptr;
	}
		
	this->numberOfUncompressedChars = stoi(numberOfUncompressedChars);

	delete numberOfDigitsInUncompressedChars;
	numberOfDigitsInUncompressedChars = nullptr;
}

vector<string> ParsedEncodedFile::parseTableFromFile(ifstream &encoded, unsigned int & j)
{
	vector<string> allFrequenciesAsStrings;
	j = 0;
	for (unsigned int i = 0; i < 256; i++)
	{
		char *numberOfDigitsInNextFrequency = new char;
		encoded.read(numberOfDigitsInNextFrequency, 1);

		unsigned int start = j;
		j++;
		string frequency;
		for (; j <= start + *numberOfDigitsInNextFrequency; j++)
		{
			char *nextChar = new char;
			encoded.read(nextChar, 1);
			frequency += *nextChar;
			delete nextChar;
			nextChar = nullptr;
		}
		allFrequenciesAsStrings.push_back(frequency);

		delete numberOfDigitsInNextFrequency;
		numberOfDigitsInNextFrequency = nullptr;
	}

	return allFrequenciesAsStrings;
}
