#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	#ifdef _WIN64
		#define CRYSTAL_PLATFORM_WINDOWS
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

#ifdef CRYSTAL_DEBUG
	#define CRYSTAL_ENABLE_ASSERTS
#endif

#ifdef CRYSTAL_ENABLE_ASSERTS
	#define CRYSTAL_ASSERT(x, ...) { if(!(x)) { CRYSTAL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CRYSTAL_CORE_ASSERT(x, ...) { if(!(x)) { CRYSTAL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CRYSTAL_ASSERT(x, ...)
	#define CRYSTAL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CRYSTAL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Crystal 
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}