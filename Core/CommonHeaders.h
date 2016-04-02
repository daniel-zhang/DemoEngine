#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <unordered_map>

#include <stdio.h>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>

#include <functional>
#include <assert.h>
#include <Windows.h>

//
// C++ delete
//
template <typename T>
void safe_delete(T*& raw_ptr)
{
    if (raw_ptr)
    {
        delete raw_ptr;
        raw_ptr = nullptr;
    }
}

//
// COM release
//
template <typename T>
void safe_release(T*& com_ptr)
{
    if (com_ptr)
    {
        com_ptr->Release();
        com_ptr = NULL;
    }
}

//
// Error check/trace
//

#ifndef show_error 
#define show_error(_msg) MessageBoxW(nullptr, (_msg).c_str(), L"Error", MB_CANCELTRYCONTINUE);
#endif

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
#ifndef vcheck
#define vcheck(_expression, _msg) (void(_expression))
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
