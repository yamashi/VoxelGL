#include "ThreadLocal.h"

THREAD_LOCAL ThreadLocal* s_pTls = nullptr;

void InitializableTLS()
{
	s_pTls = new ThreadLocal();
}

void ShutdownTLS()
{
	delete s_pTls;
}