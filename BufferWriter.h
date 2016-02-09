#pragma once

#include <ostream>

using namespace std;

class BufferWriter
{
public:
	BufferWriter();
	~BufferWriter();

	uint8_t FlushBufferToFile(ostream &outStream);
	uint8_t Write(uint8_t numberOfBits, uint32_t bits, ostream &outStream);

private:
	const uint8_t SLOTS_IN_BUFFER = 8;
	uint8_t indexOfFirstEmptyBitSlot = 0;
	uint8_t bitBuffer = 0;


	void writeBufferToFile(ostream &outStream);
};