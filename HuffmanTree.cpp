#include "stdafx.h"

#include "HuffmanTree.h"
#include <fstream>
#include <bitset>
#include <iostream>

using namespace std;

HuffmanTree::HuffmanTree()
{
	this->root = new Node;
	this->root->value = new NodeValue;
	this->root->value->weight = 0;
	this->root->value->value = 0;
}

HuffmanTree::HuffmanTree(const HuffmanTree & toCopy)
{
	*this = toCopy;
}

HuffmanTree::HuffmanTree(const FrequencyTable & toEncode)
{
	//Construct a priority queue of huffman trees (each one being just a single fv pair)
	PriorityQueue<HuffmanTree> hufQueue;
	for (unsigned int i = 0; i < toEncode.size(); i++)
	{
		FVPair next = toEncode.at(i);
		uint8_t token = next.value;
		uint16_t tokenFrequency = next.frequency;
		HuffmanTree h = HuffmanTree(token, tokenFrequency);
		hufQueue.Add(&h);
	}

	//Build one big HuffmanTree from the priority queue
	while (hufQueue.getLength() > 1)
	{
		HuffmanTree h1 = hufQueue.Remove();
		HuffmanTree h2 = hufQueue.Remove();
		HuffmanTree h3 = h1 + h2;
		hufQueue.Add(&h3);
	}

	HuffmanTree done = hufQueue.Remove();

	*this = done;
}

HuffmanTree::HuffmanTree(uint8_t rootValue, uint32_t rootWeight)
{
	this->root = new Node;
	this->root->value = new NodeValue;
	this->root->value->weight = rootWeight;
	this->root->value->value = rootValue;
}

HuffmanTree::~HuffmanTree()
{
	destroySubtree(this->root);
}

const HuffmanTree& HuffmanTree::operator+(const HuffmanTree& rhs) const
{
	HuffmanTree *result = new HuffmanTree;
	*result = *this;
	*result += rhs;
	return *result;
}

HuffmanTree& HuffmanTree::operator+=(const HuffmanTree &other)
{
	Node *otherRootCopy = copySubtree(other.root);

	Node *newRoot = new Node;
	newRoot->left = this->root;
	newRoot->right = otherRootCopy;

	newRoot->value = new NodeValue;
	newRoot->value->weight = newRoot->left->value->weight + newRoot->right->value->weight;
	this->root = newRoot;

	this->encodingIsValid = false;
	return *this;
}

bool HuffmanTree::operator>(const HuffmanTree &other) const
{
	if (this->root == nullptr)
	{
		return false;
	}
	else if (other.root == nullptr)
	{
		return true;
	}
	else
	{
		if (this->root->value->weight == other.root->value->weight)
			return this->root->value->value > other.root->value->value;
		else
			return (this->root->value->weight > other.root->value->weight);
	}
}

bool HuffmanTree::operator>=(const HuffmanTree &other) const
{
	return ((*this > other) || (*this == other));
}

bool HuffmanTree::operator<(const HuffmanTree &other) const
{
	if (this->root == nullptr)
	{
		if (other.root == nullptr)
			return false;
		else
			return true;
	}
	else if (other.root == nullptr)
	{
		return false;
	}
	else
	{
		if (this->root->value->weight == other.root->value->weight)
			return this->root->value->value < other.root->value->value;
		else
			return (this->root->value->weight < other.root->value->weight);
	}
}

bool HuffmanTree::operator<=(const HuffmanTree &other) const
{
	return ((*this < other) || (*this == other));
}

bool HuffmanTree::operator==(const HuffmanTree &other) const
{
	return equalsHelper(this->root, other.root);
}

HuffmanTree & HuffmanTree::operator=(const HuffmanTree & other)
{
	if (this != &other)
	{
		this->destroySubtree(this->root);

		this->root = copySubtree(other.root);
		this->encoding = other.encoding;
		this->encodingIsValid = other.encodingIsValid;
	}

	return *this;
}



bool HuffmanTree::Decode(vector<uint8_t> bits, uint8_t *asciiChar) const
{
	Node *whereWeAre = this->root;
	for (unsigned int i = 0; i < bits.size(); i++)
	{
		if (whereWeAre == nullptr)
			return false;
		
		if (bits.at(i) == 0)
		{
			whereWeAre = whereWeAre->left;
		}
		else
		{
			whereWeAre = whereWeAre->right;
		}
	}

	if (!((whereWeAre->left == nullptr) && (whereWeAre->right == nullptr)))//leaf node
	{
		return false;
	}
	else
	{
		*asciiChar = (whereWeAre->value->value);
		return true;
	}
}

Encoding HuffmanTree::getEncoding(bool forEncoding)
{
	if (!this->encodingIsValid)
		buildEncoding(forEncoding);

	return this->encoding;
}

void HuffmanTree::Log(const string & fileName) const
{
	ofstream log(fileName);
	logHelper(this->root, log, 0);
}




void HuffmanTree::buildEncoding(bool forEncoding)
{
	vector<uint8_t> bits;
	buildEncodingHelper(this->root, bits);

	ofstream f;
	if (forEncoding)
		f.open(name);
	else
		f.open(name2);

	for (unsigned int i = 0; i < 256; i++)
	{
		if (this->encoding.encoding[i].numberOfBits == 0)
			continue;
		else
			f << "'" << (uint8_t)i << "' : " << bitset<16>(this->encoding.encoding[i].bits) 
			<< " :" << (int)this->encoding.encoding[i].numberOfBits << endl;
	}

	this->encodingIsValid = true;
}

void HuffmanTree::buildEncodingHelper(Node * subtree, vector<uint8_t>& bits)
{
	if (subtree == nullptr)
	{
		return;
	}
	else if ((subtree->left == nullptr) && (subtree->right == nullptr))
	{
		uint8_t asciiCharForThisNode = subtree->value->value;
		this->encoding.encoding[asciiCharForThisNode].numberOfBits = bits.size();
		
		uint32_t bitValue = 0x00000000;
		unsigned int j = 0;
		for (int i = bits.size() - 1; i >= 0; i--)
		{
			bitValue |= (bits.at(j) << i);
			j++;
		}

		this->encoding.encoding[asciiCharForThisNode].bits = bitValue;
	}
	else
	{
		bits.push_back(0);
		buildEncodingHelper(subtree->left, bits);
		bits.erase(bits.begin() + (bits.size() - 1));

		bits.push_back(1);
		buildEncodingHelper(subtree->right, bits);
		bits.erase(bits.begin() + (bits.size() - 1));
	}
}

typename HuffmanTree::Node * HuffmanTree::copySubtree(const Node * const toCopy) const
{
	if (toCopy == nullptr)
	{
		return nullptr;
	}
	else
	{
		Node *copy = new Node;
		copy->value = new NodeValue;
		copy->value->value = toCopy->value->value;
		copy->value->weight = toCopy->value->weight;
		copy->left = copySubtree(toCopy->left);
		copy->right = copySubtree(toCopy->right);

		return copy;
	}
}

void HuffmanTree::destroySubtree(Node *subtree)
{
	if (subtree == nullptr)
	{
		return;
	}
	else
	{
		destroySubtree(subtree->left);
		destroySubtree(subtree->right);
		delete subtree->value;
		subtree->value = nullptr;
		delete subtree;
		subtree = nullptr;
	}
}

bool HuffmanTree::equalsHelper(const Node * const subtree, const Node * const otherSubtree) const
{
	if (subtree == nullptr)
	{
		return (otherSubtree == nullptr);
	}
	else if (otherSubtree == nullptr)
	{
		return false;
	}
	else if ((subtree->value->weight == otherSubtree->value->weight) && (subtree->value->value == otherSubtree->value->value))
	{
		return (equalsHelper(subtree->left, otherSubtree->left) && (equalsHelper(subtree->right, otherSubtree->right)));
	}
	else
	{
		return false;
	}
}

void HuffmanTree::logHelper(const Node * const subtree, ofstream & logFile, int height) const
{
	if (subtree == nullptr)
	{
		return;
	}
	else if ((subtree->left == nullptr) && (subtree->right == nullptr))
	{
		logFile << "LEAF: Height: " << height << " , value: " << subtree->value->value << endl;
	}
	else
	{
		logFile << "NODE: Height: " << height << " , weight: " << subtree->value->weight << "; ";
		logHelper(subtree->left, logFile, height + 1);
		logHelper(subtree->right, logFile, height + 1);
	}
}
