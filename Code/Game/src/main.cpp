#include "Client.h"
#include "Window.h"
#include "Shader.h"
#include "Utils.h"
#include "Log.h"
#include "Memory.h"

void Test()
{
	MEMORY_PUSH(s_pScratchPool);

	int* a = new int;
	delete a;
}

int main(int argc, char **argv)
{
	Initialize();

	Test();

	GameLoop();

	Shutdown();
	return 0;
}