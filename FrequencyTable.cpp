#include "stdafx.h"
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

FVPair FrequencyTable::at(size_t i) const
{
	return this->table.at(i);
}

uint8_t FrequencyTable::calculateNumberOfDigits(uint16_t frequency) const
{
	uint8_t numberOfDigits = 0;
	uint16_t f = frequency;
	do
	{
		numberOfDigits++;
		f /= 10;
	} while (f != 0);

	return numberOfDigits;
}

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
