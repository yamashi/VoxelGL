#include "Utils.h"

uint32_t NextPower2(uint32_t aNumber)
{
	aNumber--;
	aNumber |= aNumber >> 1;
	aNumber |= aNumber >> 2;
	aNumber |= aNumber >> 4;
	aNumber |= aNumber >> 8;
	aNumber |= aNumber >> 16;
	aNumber++;

	return aNumber;
}

uint32_t TrailingZeroes(uint32_t aNumber)
{
	static const int MultiplyDeBruijnBitPosition[32] =
	{
		0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
		31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
	};

	return MultiplyDeBruijnBitPosition[((uint32_t)((aNumber & -aNumber) * 0x077CB531U)) >> 27];
}