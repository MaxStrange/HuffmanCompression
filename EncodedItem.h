#ifndef _ENCODED_ITEM_H_
#define _ENCODED_ITEM_H_

#include "stdafx.h"
#include <iostream>

using namespace std;

struct EncodedItem
{
	uint8_t numberOfBits = 0;
	uint32_t bits = 0;
};
#endif