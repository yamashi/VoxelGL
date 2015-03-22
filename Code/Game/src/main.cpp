#include "Client.h"

int main(int argc, char **argv)
{
	Initialize();

	GameLoop();

	Shutdown();
	return 0;
}