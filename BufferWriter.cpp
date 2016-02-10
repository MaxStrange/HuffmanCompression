#include "stdafx.h"
#include "BufferWriter.h"
#include <iostream>
#include <bitset>

using namespace std;


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

	
	while (numberOfBits > 0)
	{
		uint8_t numberOfBitsOpen = SLOTS_IN_BUFFER - this->indexOfFirstEmptyBitSlot;

		uint32_t lsbsMask = ~(0xFF << numberOfBits);
		uint32_t lsbs = bits & lsbsMask;

		uint8_t numberOfBitsToWrite = (numberOfBitsOpen < numberOfBits) ? numberOfBitsOpen : numberOfBits;

		//now take the "numberOfBitsToWrite" msbs of the lsbs
		uint32_t bitsToWrite = lsbs;
		if (numberOfBits > numberOfBitsOpen)
			bitsToWrite >>= (numberOfBits - numberOfBitsOpen);
		
		uint8_t numberOfSlotsToShift = (SLOTS_IN_BUFFER - this->indexOfFirstEmptyBitSlot) - numberOfBitsToWrite;
		bitsToWrite = bitsToWrite << numberOfSlotsToShift;
		this->bitBuffer |= bitsToWrite;

		this->indexOfFirstEmptyBitSlot += numberOfBitsToWrite;

		if (this->indexOfFirstEmptyBitSlot >= SLOTS_IN_BUFFER)
			writeBufferToFile(outStream);

		numberOfBits -= numberOfBitsToWrite;
	}

	return numberOfBitsToReturn;
}

void BufferWriter::writeBufferToFile(ostream &outStream)
{
	//This operator seems to convert the integer to a string (each digit being a byte), which is why we have to use 8bit buffer
	outStream << this->bitBuffer;
	this->bitBuffer = 0x00;
	this->indexOfFirstEmptyBitSlot = 0;
}