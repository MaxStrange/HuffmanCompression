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

void Encoder::Encode(ifstream &f, const string &fName)
{
	vector<FVPair> frequencyTable;
	bool fileValid = createFrequencyTableFromFile(f, frequencyTable);
	if (fileValid)
	{
		string encodedFileName = fName + ".huf";
		ofstream encoded(encodedFileName);

		writeFrequencyTableToFile(frequencyTable, encoded);
		writeNumberOfUncompressedCharsToFile(encoded);
		writeCompressedCharsToFile(frequencyTable, encoded);

		encoded << endl;
		encoded.close();
	}
	else
	{
		cout << "File is missing or invalid." << endl;
		return;
	}
}




HuffmanTree Encoder::buildHuffmanTree(PriorityQueue<FVPair>& pq) const
{
	PriorityQueue<HuffmanTree> hufQueue;
	while (!pq.isEmpty())
	{
		FVPair next = pq.Remove();
		uint8_t token = next.value;
		uint16_t tokenFrequency = next.frequency;
		HuffmanTree h = HuffmanTree(token, tokenFrequency);
		hufQueue.Add(&h);
	}

	while (hufQueue.getLength() > 1)
	{
		HuffmanTree h1 = hufQueue.Remove();
		HuffmanTree h2 = hufQueue.Remove();
		HuffmanTree h3 = h1 + h2;
		hufQueue.Add(&h3);
	}

	HuffmanTree done = hufQueue.Remove();

	return done;
}

bool Encoder::createFrequencyTableFromFile(ifstream & f, vector<FVPair>& frequencyTable)
{
	try
	{
		frequencyTable = parse(f);
	}
	catch (exception ex)
	{
		return false;
	}
}

vector<FVPair> Encoder::parse(ifstream &f)
{
	vector<uint8_t> chars = parseToAsciiChars(f);
	this->inputFileAsAsciiChars = chars;

	uint32_t asciiCharFrequencies[256];
	for (unsigned int i = 0; i < 256; i++)
		asciiCharFrequencies[i] = 0;

	for (unsigned int i = 0; i < chars.size(); i++)
	{
		uint8_t c = chars.at(i);
		asciiCharFrequencies[c] += 1;
	}

	vector<FVPair> table;
	for (unsigned int i = 0; i < 256; i++)
	{
		if (asciiCharFrequencies[i] == 0)
			continue;

		FVPair asciiCharacterWithFrequency;
		asciiCharacterWithFrequency.frequency = asciiCharFrequencies[i];
		asciiCharacterWithFrequency.value = i;

		table.push_back(asciiCharacterWithFrequency);
	}

	return table;
}

vector<uint8_t> Encoder::parseToAsciiChars(ifstream & f) const
{
	vector<uint8_t> chars;

	string line;
	bool fileValid = false;
	while (getline(f, line))
	{
		for (unsigned int i = 0; i < line.size(); i++)
			chars.push_back(line.at(i));

		fileValid = true;
	}

	if (!fileValid)
		throw exception("File not found or invalid.");

	return chars;
}

void Encoder::writeCompressedCharsToFile(vector<FVPair> frequencyTable, ostream & encoded) const
{
	PriorityQueue<FVPair> pq;
	pq.AddEach(frequencyTable);

	HuffmanTree huff = buildHuffmanTree(pq);
	Encoding encoding = huff.getEncoding();
	BufferWriter writer;
	for (unsigned int i = 0; i < this->inputFileAsAsciiChars.size(); i++)
	{
		uint8_t c = this->inputFileAsAsciiChars.at(i);
		uint8_t numberOfBitsToWrite = encoding.encoding[c].numberOfBits;
		uint32_t bitsToWrite = encoding.encoding[c].bits;
		writer.Write(numberOfBitsToWrite, bitsToWrite, encoded);
	}

	writer.FlushBufferToFile(encoded);
}

void Encoder::writeFrequencyTableToFile(vector<FVPair> frequencyTable, ostream & outStream) const
{
	uint16_t frequenciesOfAsciiChars[256];
	for (unsigned int i = 0; i < 256; i++)
	{
		uint8_t asciiChar = i;
		
		uint16_t frequency = 0;
		//fetch the frequency that corresponds to this char
		for (unsigned int j = 0; j < frequencyTable.size(); j++)
		{
			if (frequencyTable.at(j).value == i)
			{
				frequency = frequencyTable.at(j).frequency;
				break;
			}
		}

		uint8_t numberOfDigitsInFrequency = 0;
		//calculate the number of digits in this frequency
		uint16_t f = frequency;
		do
		{
			numberOfDigitsInFrequency++;
			f /= 10;
		} while (f != 0);
		
		outStream << numberOfDigitsInFrequency;
		outStream << frequency;//converts to ascii digits when printing - so 32 turns into '3''2' in the file
	}
}

void Encoder::writeNumberOfUncompressedCharsToFile(ofstream & encoded) const
{
	uint8_t numberOfDigitsInUncompressedCharsInFile = 0;
	//calculate the number of digits in this frequency
	uint32_t numberOfUncompressedCharsInFile = this->inputFileAsAsciiChars.size();
	do
	{
		numberOfDigitsInUncompressedCharsInFile++;
		numberOfUncompressedCharsInFile /= 10;
	} while (numberOfUncompressedCharsInFile != 0);
	
	encoded << numberOfDigitsInUncompressedCharsInFile;

	numberOfUncompressedCharsInFile = this->inputFileAsAsciiChars.size();
	encoded << numberOfUncompressedCharsInFile;
}
