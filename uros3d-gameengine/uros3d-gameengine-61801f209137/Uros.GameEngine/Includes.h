#pragma once
#include <iostream>

#define MACRO_WARNING_STR(macro) "WARNING: You need to implement " macro " for this compiler"

#if defined(_MSC_VER)
//  Microsoft 
#define EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#else
// Unknown
#define EXPORT
#pragma message(MACRO_WARNING_STR("EXPORT"))
#endif

#if defined(_MSC_VER)
//  Microsoft 
#define DEPRECATED(text) __declspec(deprecated(text))
#elif defined(__GNUC__)
//  GCC
#define DEPRECATED(text) __attribute__((deprecated))
#else
// Unknown
#define DEPRECATED(text)
#pragma message(MACRO_WARNING_STR("DEPRECATED"))
#endif

#define LOG(msg) std::cout << msg << std::endl

#if defined(_DEBUG)
#define DEBUG_LOG(msg) std::cout << msg << std::endl
#else
#define DEBUG_LOG(msg)
#endif

#define ERROR_LOG(msg) std::cerr << msg << std::endl

#define IS_OF_TYPE(T1, T2) std::is_base_of<T2, T1>::value
#define NOCOPY(type) type(const type& other) = delete; type(const type&& other) = delete; type& operator=(const type&) = delete;
