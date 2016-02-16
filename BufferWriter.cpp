#include "stdafx.h"

#include <iostream>
#include <bitset>

#include "BufferWriter.h"

using namespace std;


BufferWriter::BufferWriter()
{
#if defined ENABLE_LOGS
	this->logFile = new ofstream(this->logFileName, ios::out | ios::binary);
	this->hexLog = new ofstream(this->hexLogFileName);
#endif
}


BufferWriter::~BufferWriter()
{
#if defined ENABLE_LOGS
	this->logFile->close();
	delete this->logFile;

	this->hexLog->close();
	delete this->hexLog;
#endif
}

uint8_t BufferWriter::FlushBufferToFile(ostream & outStream)
{
	uint8_t numberOfBitsWritten = this->indexOfFirstEmptyBitSlot;
	if (numberOfBitsWritten == 0)
	{
		//don't bother flushing - there's nothing in the buffer
		return 0;
	}
	else
	{
		this->writeBufferToFile(outStream);
		return numberOfBitsWritten;
	}
}

//Takes the least significant numberOfBits from bits and writes it to the file
uint8_t BufferWriter::Write(uint8_t numberOfBits, uint32_t bits, ostream &outStream)
{
	uint8_t numberOfBitsToReturn = numberOfBits;
	
	while (numberOfBits > 0)
	{
		uint8_t numberOfBitsOpen = SLOTS_IN_BUFFER - this->indexOfFirstEmptyBitSlot;

		uint32_t lsbsMask = ~(0xFF << numberOfBits);
		uint32_t lsbs = bits & lsbsMask;

		uint8_t numberOfBitsToWrite = (numberOfBitsOpen < numberOfBits) ? numberOfBitsOpen : numberOfBits;

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
	outStream << this->bitBuffer;
	
#if defined ENABLE_LOGS	
	*this->logFile << this->bitBuffer;
	*this->hexLog << dec << (this->lineCount)++ << ". 0x" << hex << int(this->bitBuffer) << endl;
#endif

	this->bitBuffer = 0x00;
	this->indexOfFirstEmptyBitSlot = 0;
}