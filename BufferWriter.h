#pragma once

#include <ostream>
#include <fstream>

using namespace std;

class BufferWriter
{
public:
	BufferWriter();
	~BufferWriter();

	uint8_t FlushBufferToFile(ostream &outStream);
	uint8_t Write(uint8_t numberOfBits, uint32_t bits, ostream &outStream);

private:
	const string logFileName = "C:\\Users\\Max\\Desktop\\Encoder\\encoderBitsLog.txt";
	ofstream *logFile = nullptr;

	const string hexLogFileName = "C:\\Users\\Max\\Desktop\\Encoder\\encoderHexLog.txt";
	ofstream *hexLog = nullptr;

	uint64_t lineCount = 0;

	//const uint8_t SLOTS_IN_BUFFER = 32;
	//uint32_t bitBuffer = 0;

	uint8_t indexOfFirstEmptyBitSlot = 0;

	const uint32_t SLOTS_IN_BUFFER = 8;
	uint8_t bitBuffer = 0;

	void writeBufferToFile(ostream &outStream);
};