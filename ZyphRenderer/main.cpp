#include "kernel.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Kernel* kernel = new Kernel();
	kernel->Run();

	return 0;
};