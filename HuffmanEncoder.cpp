// HuffmanEncoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "Encoder.h"
#include "Decoder.h"

#include "Heap.h"
#include "BufferWriter.h"

#include "CStopWatch.h"

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

	CStopWatch watch;

	string name = "C:\\Users\\Max\\Desktop\\Encoder\\toEncode.txt";
	ifstream f;
	f.open(name, ios::in | ios::binary);
	Encoder encoder;
	
	watch.StartTimer();
	encoder.Encode(f, name);
	watch.StopTimer();
	double encoderTime = watch.getElapsedTime();
	f.close();
	cout << encoderTime << endl;


	string inName = name + ".huf";
	ifstream in;
	in.open(inName, ios::in | ios::binary);
	Decoder decoder;
	watch.StartTimer();
	decoder.Decode(in, inName);
	watch.StopTimer();
	double decoderTime = watch.getElapsedTime();
	in.close();

	cout << decoderTime << endl;
	return 0;
}