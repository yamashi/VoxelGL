#pragma once

#include "Memory.h"

void InitializableTLS();
void ShutdownTLS();

struct ThreadLocal
{ 
	MemoryPoolStack memory;
};

extern THREAD_LOCAL ThreadLocal* s_pTls;