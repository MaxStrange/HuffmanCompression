#pragma once

#include <vector>
#include <fstream>
#include "HuffmanTree.h"

using namespace std;

class Decoder
{
public:
	Decoder();
	~Decoder();

	void Decode(ifstream &f, const string &fName) const;


private:
	uint32_t buildIntFromBits(vector<uint8_t> &bits) const;
	vector<uint8_t> parse(ifstream &f) const;
	vector<uint8_t> reconstructCharacter(vector<uint8_t> &bits) const;
	HuffmanTree reconstructHuffmanTree(vector<uint8_t> &bits) const;
};

