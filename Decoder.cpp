#include "stdafx.h"
#include "Decoder.h"
#include <string>
#include <vector>
#include <fstream>
#include "HuffmanTree.h"

using namespace std;

Decoder::Decoder()
{
}


Decoder::~Decoder()
{
}

void Decoder::Decode(ifstream &f, const string & fName) const
{
	vector<uint8_t> bits;
	try
	{
		bits = parse(f);		//bitsream -> each byte in the vector is a single bit from the encoded file - either a zero or one (not in ascii)
	}
	catch (exception ex)
	{
		cout << "File is missing or not valid." << endl;
		return;
	}

	HuffmanTree huff = reconstructHuffmanTree(bits);
	uint32_t numberOfSymbols = buildIntFromBits(bits);

	ofstream puff(fName + ".puff");
	while (!bits.empty())
	{
		vector<uint8_t> encodedCharacter = reconstructCharacter(bits);
		uint8_t decodedCharacter = huff.Decode(encodedCharacter);
		puff << decodedCharacter;
	}

	puff << endl;
	puff.close();
}



uint32_t Decoder::buildIntFromBits(vector<uint8_t> &bits) const
{
	//Removes the first 32 "bits" from the vector to construct a 32 bit unsigned integer and returns that integer.
	uint32_t number = 0;

	for (unsigned int i = 31; i >= 0; i++)
	{
		uint8_t bit = bits.at(i);
		bits.erase(bits.begin());

		number += bit * pow(2, i);
	}

	return number;
}

vector<uint8_t> Decoder::parse(ifstream & f) const
{
	//Returns all the bits in order from the file. By "bits" I mean that each uint8_t is either a 0 or 1 in value - 
	//not in ascii.

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
	
	vector<uint8_t> bits;
	for (unsigned int i = 0; i < chars.size(); i++)
	{
		for (unsigned int j = 0; j < 8; j++)
		{
			uint8_t ch = chars.at(i);
			uint8_t bit = (ch >> j) & 0x01;
			bits.push_back(bit);
		}
	}

	return bits;
}

vector<uint8_t> Decoder::reconstructCharacter(vector<uint8_t> &bits) const
{
	//TODO : figure out how to do this
	return vector<uint8_t>();
}

HuffmanTree Decoder::reconstructHuffmanTree(vector<uint8_t> &bits) const
{
	//Removes the first however many "bits" from the vector and builds the tree
	return HuffmanTree(5, 5);
}
