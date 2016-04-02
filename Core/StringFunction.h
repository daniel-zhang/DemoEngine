#pragma once

#include "CommonHeaders.h"

std::string ws2s(const std::wstring& ws);
std::wstring s2ws(const std::string& s);

#pragma warning(push) 
// error C4996: '_snprintf': This function or variable may be unsafe. Consider using _snprintf_s instead.
#pragma warning(disable : 4996)

// ANSI version
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    std::size_t size = _snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    _snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
// UNICODE version 
template<typename ... Args>
std::wstring wstring_format(const std::wstring& format, Args ... args)
{
    std::size_t size = _snwprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
    _snwprintf(buf.get(), size, format.c_str(), args ...);
    return std::wstring(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
#pragma warning(pop)
