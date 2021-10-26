#pragma once

// Prevent C++ name mangling, as this would prevent loadSymbol from getting the right symbols
extern "C"
{
	__declspec(dllexport) void foo();

	__declspec(dllexport) extern int bar;
}