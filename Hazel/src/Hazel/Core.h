#pragma once

// Preprocessor directives for importing dll classes/functions from Hazel
#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else  
		#define HAZEL_API __declspec(dllimport)
	#endif
#else
#error Hazel only supports Windows
#endif

// define for bitwise shifting for the event category class
#define BIT(x) (1 << x)