#pragma once

#include <vector>

#include "FVPair.h"

using namespace std;

class FrequencyTable
{
public:
	FrequencyTable();
	FrequencyTable(uint32_t asciiFrequencies[]);
	~FrequencyTable();

	uint16_t getFrequency(uint8_t asciiChar) const;

	FVPair at(size_t i) const;
	uint8_t calculateNumberOfDigits(uint16_t frequency) const;
	void push_back(const FVPair &item);
	size_t size() const;
	vector<FVPair> toVector() const;

private:
	vector<FVPair> table;
};

