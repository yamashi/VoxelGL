#include "Client.h"
#include "Window.h"
#include "Shader.h"
#include "Utils.h"
#include "Log.h"

#include <chrono>

#include "SDL.h"

int main(int argc, char **argv)
{
	Initialize();

	int c1, c2;

	auto t = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1000000; ++i)
	{
		int* p = new int;
		char* p2 = new char[100];
		delete p2;
		delete p;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	c1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t).count();

	t = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1000000; ++i)
	{
		int* p = (int*)malloc(sizeof(int));
		void* p2 = malloc(100);
		free(p2);
		free(p);
	}

	t2 = std::chrono::high_resolution_clock::now();

	c2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t).count();

	std::cout << c1 << " " << c2 << std::endl;


	GameLoop();

	Shutdown();
	return 0;
}