#pragma once
#include <memory>

// Preprocessor directives for importing dll classes/functions from Orca
#ifdef OA_PLATFORM_WINDOWS
#if OA_DYNAMIC_LINK
	#ifdef OA_BUILD_DLL
		#define ORCA_API __declspec(dllexport)
	#else  
		#define ORCA_API __declspec(dllimport)
	#endif
#else
	#define ORCA_API
#endif
#else 
	#error Orca only supports Windows
#endif

#ifdef OA_DEBUG
#define OA_ENABLE_ASSERTS
#endif

#ifdef OA_ENABLE_ASSERTS
	#define OA_ASSERT(x, ...) { if(!(x)) { OA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define OA_CORE_ASSERT(x, ...) { if(!(x)) { OA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define OA_ASSERT(x, ...) 
	#define OA_CORE_ASSERT(x, ...)
#endif


// define for bitwise shifting for the event category class
#define BIT(x) (1 << x)

//macro for binding functions
#define OA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Orca
{
	// Scope wrapper for unique smart pointers
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// Ref wrapper for shared smart pointers
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

	

	


