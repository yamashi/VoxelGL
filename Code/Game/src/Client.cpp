#include "Client.h"
#include "Renderer.h"
#include "Log.h"
#include "Memory.h"

void Initialize()
{
	InitializeLog();
	InitializeRenderer();
}

void Shutdown()
{
	ShutdownRenderer();
	ShutdownLog();
}