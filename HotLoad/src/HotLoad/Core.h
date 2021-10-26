#pragma once

// Export the DLL for the library (hl needs to define this when building)
#ifdef HL_BUILD_DLL
	#define HOTLOAD_API __declspec(dllexport)
#else
	// This is meant for the application (game etc). It should not define this
	#define HOTLOAD_API __declspec(dllimport)
#endif