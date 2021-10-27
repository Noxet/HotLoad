#pragma once

#include <HotLoad.h>

// Export the DLL for the library (hl needs to define this when building)
#ifdef TESTLIB_BUILD_DLL
#define TESTLIB_API __declspec(dllexport)
#else
	// This is meant for the application. It should not define this
#define TESTLIB_API __declspec(dllimport)
#endif



// Prevent C++ name mangling, as this would prevent loadSymbol from getting the right symbols
extern "C"
{
	TESTLIB_API void foo();

	TESTLIB_API extern int bar;
}

std::array<const char*, 2> g_exports = { "foo", "bar" };

/*
 * Declare a class to extend the HotLoadModule, and set the instance to ourselves
 */
class TESTLIB_API TestModule : public HotLoad::HotLoadModule<TestModule, g_exports.size()>
{
public:
	static void Foo()
	{
		getInstance().execute<void>("foo");
	}

	static int getBar()
	{
		// decltype resolves to the datatype of bar, i.e., int
		return *getInstance().getVar<decltype(bar)>("bar");
	}

protected:
	virtual const char* getPath() const override
	{
		return "HotLoadTest.dll";
	}

	virtual std::array<const char*, g_exports.size()>& getSymbolNames() const override
	{
		return g_exports;
	}
};