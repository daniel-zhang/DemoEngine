#include "Util.h"
#include <codecvt> // For wide/ascii char convert;

std::string ws2s(const std::wstring& ws)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(ws);
}

std::wstring s2ws(const std::string& s)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.from_bytes(s);
}
