#include "stdafx.h"
#include "ParsedEncodedFile.h"
#include <fstream>
#include <iostream>
#include <string>
#include "FVPair.h"
#include <string>

using namespace std;

ParsedEncodedFile::ParsedEncodedFile()
{
	for (unsigned int i = 0; i < 256; i++)
		this->asciiFrequencies[i] = 0;
}

ParsedEncodedFile::~ParsedEncodedFile()
{
}

vector<uint8_t> ParsedEncodedFile::getBits() const
{
	return this->bits;
}

vector<FVPair> ParsedEncodedFile::getFrequencyTable() const
{
	vector<FVPair> frequencyTable;
	for (unsigned int i = 0; i < 256; i++)
	{
		uint16_t frequency = this->asciiFrequencies[i];
		if (frequency == 0)
			continue;

		uint8_t value = i;
		FVPair fv;
		fv.frequency = frequency;
		fv.value = value;

		frequencyTable.push_back(fv);
	}

	return frequencyTable;
}

uint32_t ParsedEncodedFile::getNumberOfUncompressedChars() const
{
	return this->numberOfUncompressedChars;
}

bool ParsedEncodedFile::Parse(ifstream & encoded)
{
	vector<char> allLines(istreambuf_iterator<char>(encoded), (istreambuf_iterator<char>()));

	//bool fileValid = false;
	//vector<string> lines;
	//string line;
	//while (getline(encoded, line))
	//{
	//	lines.push_back(line);

	//	fileValid = true;
	//}

	//if (!fileValid)
	//	return false;
	//else
	//	encoded.close();

	//string allLines;
	//for (unsigned int i = 0; i < lines.size(); i++)
	//{
	//	allLines += lines.at(i);
	//	static const uint8_t linefeed = 10;
	//	
	//	//if this is not the last line, the line separator is due to a \n, which is just an ascii 10
	//	if (i + 1 != lines.size())
	//		allLines += linefeed;
	//}

	//Now have the whole compressed file in one long string

	vector<string> allFrequencies;
	unsigned int j = 0;
	for (unsigned int i = 0; i < 256; i++)
	{
		//take the next byte, it is how many DIGITS there are in the next frequency
		//take that many bytes and combine them using stoi to form the frequency
		uint8_t numberOfDigitsInNextFrequency = allLines.at(j);
		unsigned int start = j;
		j++;
		string frequency;
		for (; j <= start + numberOfDigitsInNextFrequency; j++)
		{
			frequency += allLines.at(j);
		}
		allFrequencies.push_back(frequency);
	}

	//this->frequencies = allFrequencies
	for (unsigned int i = 0; i < allFrequencies.size(); i++)
	{
		uint16_t frequency = stoi(allFrequencies.at(i));
		this->asciiFrequencies[i] = frequency;
	}
	

	uint8_t numberOfDigitsInUncompressedChars = allLines.at(j);
	unsigned int start = j;
	j++;
	string numberOfUncompressedChars;
	for (; j <= start + numberOfDigitsInUncompressedChars; j++)
		numberOfUncompressedChars += allLines.at(j);

	this->numberOfUncompressedChars = stoi(numberOfUncompressedChars);




	vector<uint8_t> bytes;
	for (; j < allLines.size(); j++)
	{
		bytes.push_back(allLines.at(j));
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
	
	return true;
}
