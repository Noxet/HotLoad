
#include <iostream>
#include <HotLoad.h>


const char* g_libPath = "HotLoadTest.dll";


void (*foo) ();

int main()
{
	if (void* handle = HotLoad::load(g_libPath))
	{
		// cast to a function pointer, to call the function
		foo = reinterpret_cast<void (*)()>(HotLoad::loadSymbol(handle, "foo"));
		foo();

		// read data from global var
		int bar = *static_cast<int*>(HotLoad::loadSymbol(handle, "bar"));
		std::cout << "bar = " << bar << std::endl;

		std::cout << "Change the test lib, recompile, and press enter." << std::flush;
		while (std::cin.get() != '\n') {}

		// reload the testlib live!
		HotLoad::reload(handle, g_libPath);
		std::cout << "After hot reloading" << std::endl;

		// we need to refetch the symbols, cause their addresses may have changed.
		foo = reinterpret_cast<void (*)()>(HotLoad::loadSymbol(handle, "foo"));
		foo();

		bar = *static_cast<int*>(HotLoad::loadSymbol(handle, "bar"));
		std::cout << "bar = " << bar << std::endl;

		while (std::cin.get() != '\n') {}
	}
	else
	{
		std::cout << "Failed to open DLL" << std::endl;
	}

	return 0;
}
