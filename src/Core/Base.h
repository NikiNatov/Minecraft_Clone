#pragma once

#include <memory>

#ifdef _DEBUG
	#define ASSERT(x,...) { if(!(x)) { std::cout << "Assertion failed: " << __VA_ARGS__ << std::endl; __debugbreak();}}
#else
	#define ASSERT(x,...)
#endif

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
Ref<T> CreateRef(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using ScopedPtr = std::unique_ptr<T>;

template<typename T, typename... Args>
ScopedPtr<T> CreateScoped(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}