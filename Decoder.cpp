#include "stdafx.h"

#include <string>
#include <vector>
#include <fstream>

#include "HuffmanTree.h"
#include "ParsedEncodedFile.h"
#include "PriorityQueue.h"
#include "FVPair.h"
#include "CStopWatch.h"

#include "Decoder.h"


using namespace std;

Decoder::Decoder()
{
#if defined ENABLE_LOGS
	this->logFile = new ofstream(this->bitsLogFileName, ios::out | ios::binary);
	this->hexLog = new ofstream(this->hexLogFileName);
#endif
}

Decoder::~Decoder()
{
#if defined ENABLE_LOGS
	this->logFile->close();
	delete this->logFile;

	this->hexLog->close();
	delete this->hexLog;
#endif
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
#if defined ENABLE_LOGS
	huff.Log(huffTreeLogFileName);
#endif
	Encoding enc = huff.getEncoding(false);
	string name = parsedFile.getFileName() + ".puff";
	ofstream decoded(name, ios::out | ios::binary);
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

	uint32_t bitsToCheck = 0;

	uint32_t numberDecodedSoFar = 0;
	uint8_t largestNumberOfBitsToEncodeACharacter = getLargestNumberOfBitsToEncode(tree.getEncoding(false));
	uint8_t smallestNumberOfBitsToEncodeACharacter = getSmallestNumberOfBitsToEncode(tree.getEncoding(false));

	int numberOfBitsAvailable = 0;//from buffer
	unsigned int numberOfBitsAccumulated = 0;
	uint32_t bits = 0;
	char nextByte;
	uint8_t asciiChar = 0;


#if defined ENABLE_LOGS
	const string fileName = "C:\\Users\\Max\\Desktop\\Encoder\\numberOfCharsDecodedLog.txt";
	ofstream decodedLog(fileName);
#endif



	CStopWatch watch;
	
	while (numberDecodedSoFar < numberOfUncompressedChars)
	{//for each byte in the file

		/*
		Collect the next 8 bits from the file and add them on to the bits buffer
		*/
		if ((pos != end) && (numberOfBitsAvailable == 0))
		{
			encoded.read(&nextByte, 1);
			pos++;
			numberOfBitsAvailable = 8;

#if defined ENABLE_LOGS
			uint8_t logByte = nextByte;
			*this->logFile << logByte;
			*this->hexLog << dec << (this->lineCount)++ << ". 0x" << hex << int(logByte) << endl;
#endif
		}

		bits <<= 1;
		bits |= (nextByte & 0x80) >> 7;
		nextByte <<= 1;

		numberOfBitsAccumulated++;
		numberOfBitsAvailable--;
		
		if (numberOfBitsAccumulated < smallestNumberOfBitsToEncodeACharacter)
			continue;

		bool successfullyDecoded = tree.Decode(bits, numberOfBitsAccumulated, &asciiChar);
		if (successfullyDecoded)
		{
			bits = 0;
			numberOfBitsAccumulated = 0;
			numberDecodedSoFar++;

			decoded.write((char *)&asciiChar, 1);
		}
	}
#if defined ENABLE_LOGS
	decodedLog.close();
#endif
}

vector<uint8_t> Decoder::parseCharToBits(char c) const
{
	vector<uint8_t> bits;
	for (int i = 7; i >= 0; i--)
		bits.push_back((c >> i) & 0x01);
	return bits;
}