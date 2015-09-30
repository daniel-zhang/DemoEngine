#pragma once


//
// String manipulation
//
#include <string>

std::string ws2s(const std::wstring& ws);
std::wstring s2ws(const std::string& s);

//
// Custom hash
//
#include <functional>

template<typename T>
void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

//
// Error check/trace
//
#include <Windows.h>
#include <assert.h>


// Simple version
#if defined(_DEBUG) || defined(DEBUG)
#ifndef check
#define check(_expression)\
    (void)(!!(_expression) || (_wassert(_CRT_WIDE(#_expression), _CRT_WIDE(__FILE__), __LINE__), 0))
#endif
#else
#ifndef check
#define check(_expression) (_expression)
#endif
#endif

// Verbose version with msg output
#if defined(_DEBUG) || defined(DEBUG)
#ifndef vcheck
#define vcheck(_expression, _msg)\
    (void)(!!(_expression) || (_wassert(_CRT_WIDE(#_expression L"\nMessage: " _msg), _CRT_WIDE(__FILE__), __LINE__), 0))
#endif
#else
#ifndef check
#define check(_expression, _msg) (void(_expression))
#endif
#endif

// HR simple version
#if defined(DEBUG) || defined(_DEBUG)
#ifndef check_hr
#define check_hr(_expression)\
    (void)(!!SUCCEEDED(_expression) || (_wassert(_CRT_WIDE(#_expression), _CRT_WIDE(__FILE__), __LINE__), 0))
#endif
#else
    #ifndef check_hr
    #define check_hr(x) (x)
    #endif
#endif

// HR verbose version
#if defined(DEBUG) || defined(_DEBUG)
#ifndef vcheck_hr
#define vcheck_hr(_expression, _msg)\
    (void)(!!SUCCEEDED(_expression) || (_wassert(_CRT_WIDE(#_expression L"\nMessage: " _msg), _CRT_WIDE(__FILE__), __LINE__), 0))
#endif
#else
    #ifndef vcheck_hr
    #define vcheck_hr(x, y) (x)
    #endif
#endif
