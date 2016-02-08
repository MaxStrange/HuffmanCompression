#include "stdafx.h"
#include "BufferWriter.h"


BufferWriter::BufferWriter()
{
}


BufferWriter::~BufferWriter()
{
}

uint8_t BufferWriter::FlushBufferToFile(ostream & outStream)
{
	uint8_t numberOfBitsWritten = this->indexOfFirstEmptyBitSlot;
	this->writeBufferToFile(outStream);
	return numberOfBitsWritten;
}

//Takes the least significant numberOfBits from bits and writes it to the file
uint8_t BufferWriter::Write(uint8_t numberOfBits, uint32_t bits, ostream &outStream)
{
	uint8_t numberOfBitsToReturn = numberOfBits;
	/*
	1. Take the "numberOfBits" from the least significant end of "bits"
	2. Take the number of bit slots left in the buffer from the those bits
	3. Shift the bits left the right number of slots
	4. buffer |= those bits
	5. If the buffer is full, write it to the file
	6. Subtract however many bits you ORd with the buffer from the lsbs
	7. Go to step 2, but with the lsbs from step 6
	*/

	uint8_t numberOfBitsOpen = SLOTS_IN_BUFFER - this->indexOfFirstEmptyBitSlot;
	
	while (numberOfBits > 0)
	{
		uint32_t lsbsMask = ~(0xFFFFFFFF << numberOfBits);
		uint32_t lsbs = bits & lsbsMask;

		uint8_t numberOfBitsToWrite = (numberOfBitsOpen < numberOfBits) ? numberOfBitsOpen : numberOfBits;
		numberOfBits -= numberOfBitsToWrite;
		//now take the "numberOfBitsToWrite" msbs of the lsbs
		uint32_t bitsToWrite = lsbs;
		if (numberOfBitsToWrite > numberOfBitsOpen)
			bitsToWrite >>= numberOfBitsToWrite;
		
		uint8_t numberOfSlotsToShift = (SLOTS_IN_BUFFER - this->indexOfFirstEmptyBitSlot) - numberOfBitsToWrite;
		bitsToWrite = bitsToWrite << numberOfSlotsToShift;
		this->bitBuffer |= bitsToWrite;

		this->indexOfFirstEmptyBitSlot += numberOfBitsToWrite;

		if (this->indexOfFirstEmptyBitSlot >= SLOTS_IN_BUFFER)
			writeBufferToFile(outStream);
	}

	return numberOfBitsToReturn;
}

void BufferWriter::writeBufferToFile(ostream &outStream)
{
	outStream << this->bitBuffer;
	this->bitBuffer = 0x00000000;
	this->indexOfFirstEmptyBitSlot = 0;
}
