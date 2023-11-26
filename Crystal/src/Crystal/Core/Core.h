#pragma once
#include <memory>

/* A tonnn of preprossessor macros to be used later in the engine */
// Platform detection using predefined macros
#ifdef _WIN32
	#ifdef _WIN64
		//#define CRYSTAL_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Simulator not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define CRYSTAL_PLATFORM_IOS
		#error "IOS not supported!"
	#elif TARGET_OS_MAC == 1
		#define CRYSTAL_PLATFORM_MACOS
		#error "MacOS not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elifd defined(__ANDROID__)
	#define CRYSTAL_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define CRYSTAL_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif
/* -- Platform Macros -- */

#ifdef CRYSTAL_DEBUG
	#define CRYSTAL_ENABLE_ASSERTS
#endif
/* -- Debug Macros -- */

/* -- Assert macros -- */
#define CRYSTAL_ENABLE_ASSERTS

#ifdef CRYSTAL_ENABLE_ASSERTS
	#define CRYSTAL_ASSERT(x, ...) { if(!(x)) { CRYSTAL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CRYSTAL_CORE_ASSERT(x, ...) { if(!(x)) { CRYSTAL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CRYSTAL_ASSERT(x, ...)
	#define CRYSTAL_CORE_ASSERT(x, ...)
#endif
/* -- Assert macros -- */

/* Bit shifting macro */
#define BIT(x) (1 << x)

/* Bind event macro */
#define CRYSTAL_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Crystal 
{
	/* Templated macro for Scope that turns Scope into a unique pointer */
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	/* Templated macro for Ref that turns Ref into a shared pointer */
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
