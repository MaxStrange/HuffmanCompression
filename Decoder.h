#pragma once

#include <vector>
#include <fstream>
#include "HuffmanTree.h"
#include "ParsedEncodedFile.h"
#include "PriorityQueue.h"

using namespace std;

class Decoder
{
public:
	Decoder();
	~Decoder();

	void Decode(ifstream &f, const string &fName);


private:
	const string huffTreeLogFileName = "C:\\Users\\Max\\Desktop\\Encoder\\decoderHuffTree.txt";
	ofstream *logFile = nullptr;
	const string bitsLogFileName = "C:\\Users\\Max\\Desktop\\Encoder\\decoderBitsLog.txt";
	const string hexLogFileName = "C:\\Users\\Max\\Desktop\\Encoder\\decoderHexLog.txt";
	ofstream *hexLog = nullptr;

	uint64_t lineCount = 0;

	uint8_t getLargestNumberOfBitsToEncode(const Encoding &encoding) const;
	uint8_t getSmallestNumberOfBitsToEncode(const Encoding &encoding) const;
	void decodeInputFile(ParsedEncodedFile &parsedFile, ifstream &encodedFile);
	void decompressCharacters(HuffmanTree &tree, ifstream &encoded, uint32_t numberOfUncompressedChars, ofstream &decoded);
	vector<uint8_t> parseCharToBits(char c) const;
};

