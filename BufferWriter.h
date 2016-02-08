#pragma once

#include <ostream>

using namespace std;

class BufferWriter
{
public:
	BufferWriter();
	~BufferWriter();

	uint8_t FlushBufferToFile(ostream &outStream);
	uint8_t Write(uint8_t numberOfBits, uint32_t bits, ostream &outStream);		//Takes numberOfBits from the bottom of bits and writes it to its own buffer (then returns how many bits it actually wrote - which should match)
															//When the buffer is full, flush it to the file - NOTE that the last byte may have some padding on it.
private:
	const uint8_t SLOTS_IN_BUFFER = 32;
	uint8_t indexOfFirstEmptyBitSlot = 0;
	uint32_t bitBuffer = 0;


	void writeBufferToFile(ostream &outStream);
};


/*
Encoding table is an array of 256 structures, each one being the number of bits and the actual bits,
so that you would do this:
if 'a' is encoded as 11011,
every time the file reader sees an 'a', it would pass this to the buffer writer: 
write(encoding_table['a'].num_bits, encoding_table['a'].bits);

Notice that this means that "bits" is 27 (which is 0001 1011) and "num_bits" is 5 (the bufwriter wants to take the
least significant 5 bits from the number 27).
*/
