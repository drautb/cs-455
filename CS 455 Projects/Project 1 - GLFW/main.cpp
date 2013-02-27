#include <iostream>

#include "cs455Utils.h"
#include "Window.h"
#include "TestSuite.h"

int main(int argc, char *argv[])
{
	//TestSuite::RunAll(std::cout);

	Window *window = new Window();

	//TestSuite::TestP3LoadMatrixD(cout, window);

	if (window->Open())
		window->EnterMainLoop();
	else
	{
		SAFE_DELETE(window);
		return -1;
	}

	SAFE_DELETE(window);

	return 0;
}
