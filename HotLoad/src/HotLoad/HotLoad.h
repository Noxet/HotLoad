#pragma once

#include "Core.h"

#include <Windows.h>

namespace HotLoad
{
	class HOTLOAD_API Foo
	{
	public:
		int getTheAnswer() const;

	private:
		int m_answer = 42;
	};

	/*
	 * Define the API for handling the DLLs. These are similar to POSIX dl* functions
	 */
	HOTLOAD_API void* load(const char* filepath);
	HOTLOAD_API void* loadSymbol(void* library, const char* symbol);
	HOTLOAD_API void reload(void* library, const char* filepath);
	HOTLOAD_API void unload(void* library);
	HOTLOAD_API void printError();
}
