#pragma once

uint32_t NextPower2(uint32_t aNumber);
uint32_t TrailingZeroes(uint32_t aNumber);

#ifndef BUFFER_OFFSET
#	define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif