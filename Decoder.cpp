#include "stdafx.h"
#include "Decoder.h"
#include <string>
#include <vector>
#include <fstream>
#include "HuffmanTree.h"
#include "ParsedEncodedFile.h"
#include "PriorityQueue.h"
#include "FVPair.h"

using namespace std;

Decoder::Decoder()
{
}


Decoder::~Decoder()
{
}

void Decoder::Decode(ifstream &f, const string & fName) const
{
	ParsedEncodedFile parsedFile;
	bool fileValid = parsedFile.Parse(f);
	if (fileValid)
	{
		PriorityQueue<FVPair> pq;
		pq.AddEach(parsedFile.getFrequencyTable());

		HuffmanTree huff = buildHuffmanTree(pq);
		ofstream decoded(fName + ".puff");
		
		vector<uint8_t> uncompressedAsciiChars = decompressCharacters(huff, parsedFile);
		for (unsigned int i = 0; i < uncompressedAsciiChars.size(); i++)
			decoded << uncompressedAsciiChars.at(i);
		decoded << endl;
		decoded.close();
	}
	else
	{
		cout << "File is invalid or missing." << endl;
		return;
	}
}



HuffmanTree Decoder::buildHuffmanTree(PriorityQueue<FVPair>& pq) const
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

vector<uint8_t> Decoder::decompressCharacters(const HuffmanTree & tree, const ParsedEncodedFile & encoded) const
{
	vector<uint8_t> asciiChars;

	vector<uint8_t> bits = encoded.getBits();
	vector<uint8_t> bitsToCheck;
	unsigned int i = 0;
	while (i < bits.size())
	{
		bitsToCheck.push_back(bits.at(i));

		uint8_t asciiChar = 0;
		if (tree.Decode(bitsToCheck, &asciiChar))
		{
			bitsToCheck.clear();
			asciiChars.push_back(asciiChar);
		}

		i++;
	}

	return asciiChars;
}
