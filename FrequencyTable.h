#ifndef _FREQUENCY_TABLE_H_
#define _FREQUENCY_TABLE_H_

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
	uint32_t getTotal() const;
	void setTotal(uint32_t total);

	FVPair at(size_t i) const;
	uint8_t calculateNumberOfDigits(uint64_t num) const;
	vector<char> getDigits(uint64_t num) const;
#if defined ENABLE_LOGS
	void Log(string fileName) const;
#endif
	void push_back(const FVPair &item);
	size_t size() const;
	vector<FVPair> toVector() const;

private:
	vector<FVPair> table;
	uint32_t total = 0;
};

#endif