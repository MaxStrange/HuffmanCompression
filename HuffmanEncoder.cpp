// HuffmanEncoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "Encoder.h"

#include "Heap.h"

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
	f.open(name);
	Encoder encoder;
	encoder.Encode(f, name);


	return 0;
}