#include "Client.h"
#include "Renderer.h"
#include "Log.h"

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