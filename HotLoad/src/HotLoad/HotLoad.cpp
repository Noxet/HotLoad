
#include "HotLoad.h"

#include <iostream>

namespace HotLoad
{
	int Foo::getTheAnswer() const
	{
		return m_answer;
	}


	void* load(const char* filepath)
	{
		/*
		 * Loads the DLL into memory.
		 * https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya
		 */
		return LoadLibrary(filepath);
	}


	void* loadSymbol(void* library, const char* symbol)
	{
		/*
		 * Gets the address of an exported function or variable.
		 * https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
		 */
		return static_cast<void*>(GetProcAddress(static_cast<HMODULE>(library), symbol));
	}


	void reload(void* library, const char* filepath)
	{
		unload(library);
		library = load(filepath);
	}


	void unload(void* library)
	{
		/*
		 * Frees the DLL module.
		 * https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-freelibrary
		 */
		FreeLibrary(static_cast<HMODULE>(library));
	}


	void printError()
	{
		std::cout << "Error: " << std::endl;
	}
}
