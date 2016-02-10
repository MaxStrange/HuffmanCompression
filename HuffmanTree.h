#pragma once

#include "stdafx.h"

#include <string>
#include <vector>

#include "Encoding.h"
#include "NodeValue.h"
#include "PriorityQueue.h"
#include "FrequencyTable.h"

using namespace std;

class HuffmanTree
{
public:
	HuffmanTree();
	HuffmanTree(const HuffmanTree &toCopy);
	HuffmanTree(const FrequencyTable &toEncode);
	HuffmanTree(uint8_t rootValue, uint32_t rootWeight);
	~HuffmanTree();

	const HuffmanTree& operator+(const HuffmanTree &rhs) const;
	HuffmanTree& operator+=(const HuffmanTree &other);
	bool operator>(const HuffmanTree &other) const;
	bool operator>=(const HuffmanTree &other) const;
	bool operator<(const HuffmanTree &other) const;
	bool operator<=(const HuffmanTree &other) const;
	bool operator==(const HuffmanTree &other) const;
	HuffmanTree& operator=(const HuffmanTree &other);

	bool Decode(vector<uint8_t> bits, uint8_t *asciiChar) const;
	Encoding getEncoding(bool forEncoding);

private:
	struct Node
	{
		NodeValue *value = nullptr;
		Node *left = nullptr;
		Node *right = nullptr;
	};

	Node *root = nullptr;
	bool encodingIsValid = false;
	Encoding encoding;

	void buildEncoding(bool forEncoding);
	void buildEncodingHelper(Node *subtree, vector<uint8_t> &bits);
	Node* copySubtree(const Node * const toCopy) const;
	void destroySubtree(Node *subtree);
	bool equalsHelper(const Node * const subtree, const Node * const otherSubtree) const;

	string name = "C:\\Users\\Max\\Desktop\\logEncoding.txt";
	string name2 = "C:\\Users\\Max\\Desktop\\logDecoding.txt";
};
