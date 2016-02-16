#include "stdafx.h"

#include <fstream>

#include "FrequencyTable.h"


FrequencyTable::FrequencyTable()
{
}

FrequencyTable::FrequencyTable(uint32_t asciiFrequencies[])
{
	for (unsigned int i = 0; i < 256; i++)
	{
		if (asciiFrequencies[i] == 0)
			continue;

		FVPair asciiCharacterWithFrequency;
		asciiCharacterWithFrequency.frequency = asciiFrequencies[i];
		asciiCharacterWithFrequency.value = i;

		this->table.push_back(asciiCharacterWithFrequency);
	}
}


FrequencyTable::~FrequencyTable()
{
}

uint16_t FrequencyTable::getFrequency(uint8_t asciiChar) const
{
	for (unsigned int j = 0; j < this->table.size(); j++)
	{
		if (this->table.at(j).value == asciiChar)
			return this->table.at(j).frequency;
	}
	return 0;
}

uint32_t FrequencyTable::getTotal() const
{
	return this->total;
}

void FrequencyTable::setTotal(uint32_t total)
{
	this->total = total;
}

FVPair FrequencyTable::at(size_t i) const
{
	return this->table.at(i);
}

vector<char> FrequencyTable::getDigits(uint64_t num) const
{
	vector<char> digits;
	uint8_t numDigs = calculateNumberOfDigits(num);
	for (int i = numDigs - 1; i >= 0; i--) {
		int y = pow(10, i);
		int z = num / y;
		int x = num / (y * 10);
		int digit = z - x * 10;
		digits.push_back(digit);
	}
	return digits;
}

uint8_t FrequencyTable::calculateNumberOfDigits(uint64_t num) const
{
	uint8_t numberOfDigits = 0;
	do
	{
		numberOfDigits++;
		num /= 10;
	} while (num != 0);

	return numberOfDigits;
}

#if defined ENABLE_LOGS
void FrequencyTable::Log(string fileName) const
{
	ofstream f(fileName);
	for (unsigned int i = 0; i < this->table.size(); i++)
	{
		f << "Value: " << this->table.at(i).value << " : " << this->table.at(i).frequency << endl;
	}
}
#endif

void FrequencyTable::push_back(const FVPair &item)
{
	this->table.push_back(item);
}

size_t FrequencyTable::size() const
{
	return this->table.size();
}

vector<FVPair> FrequencyTable::toVector() const
{
	return this->table;
}
