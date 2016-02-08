#pragma once
#include <iostream>

struct FVPair
{
	uint16_t frequency = 0;
	uint8_t value;

	bool operator>(const FVPair &other) const;
	bool operator>=(const FVPair &other) const;
	bool operator<(const FVPair &other) const;
	bool operator<=(const FVPair &other) const;
	bool operator==(const FVPair &other) const;
};

inline bool FVPair::operator>(const FVPair& other) const
{
	if (frequency > other.frequency)
	{
		return true;
	}
	else if (frequency < other.frequency)
	{
		return false;
	}
	else
	{
		return (value > other.value);
	}
}

inline bool FVPair::operator>=(const FVPair& other) const
{
	return ((*this > other) || (*this == other));
}

inline bool FVPair::operator<(const FVPair& other) const
{
	if (frequency < other.frequency)
	{
		return true;
	}
	else if (frequency > other.frequency)
	{
		return false;
	}
	else
	{
		return (value < other.value);
	}
}

inline bool FVPair::operator<=(const FVPair& other) const
{
	return ((*this < other) || (*this == other));
}

inline bool FVPair::operator==(const FVPair& other) const
{
	return ((frequency == other.frequency) && (value == other.value));
}
