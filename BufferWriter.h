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

	const uint8_t SLOTS_IN_BUFFER = 8;
	uint8_t indexOfFirstEmptyBitSlot = 0;
	uint8_t bitBuffer = 0;


	void writeBufferToFile(ostream &outStream);
};