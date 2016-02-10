// HuffmanEncoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "Encoder.h"
#include "Decoder.h"

#include "Heap.h"
#include "BufferWriter.h"

using namespace std;

int main(int argc, char *argv[])
{
	//ifstream f;
	//if (argc != 2)
	//{
	//	cout << "USAGE: Compress FILE_NAME" << endl;
	//}
	//else
	//{
	//	string name = argv[1];
	//	f.open(name);
	//	Encoder encoder;
	//	encoder.Encode(f, name);
	//}

	string name = "C:\\Users\\Max\\Desktop\\toEncode.txt";
	ifstream f;
	f.open(name, ios::in | ios::binary);
	Encoder encoder;
	encoder.Encode(f, name);

	string oName = name + ".huf";
	ifstream o;
	o.open(oName, ios::in | ios::binary);
	Decoder decoder;
	decoder.Decode(o, oName);


	return 0;
}