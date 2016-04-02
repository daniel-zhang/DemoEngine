#include "Log.h"

/*Temporary objects get destroyed as soon as they are out of the expression they live in*/
Log::~Log()
{
    std::ofstream& ofs = Log::getFileStream();
    if (ofs.good())
    {
        ofs << mLineBuffer.str() << std::endl;
        ofs.flush();
    }
}

std::ostringstream& Log::getLineBuffer(ELogLevel level)
{
    const static char* headChar[] = { "X ", "! ", "> ", "# " };
    const static char* levelString[] = {
        " ERROR ",
        " WARNING ",
        " INFO ",
        " DEBUG " };

    mLineBuffer << headChar[level] << getDateTime() << levelString[level];
    return mLineBuffer;
}

void Log::setPath(const std::string& inPath, bool append /*= true*/)
{
    std::ofstream& ofs = Log::getFileStream();
    ofs.close();
    if (append)
        ofs.open(inPath, std::ofstream::out | std::ofstream::app);
    else
        ofs.open(inPath);
}

std::ofstream& Log::getFileStream()
{
    static std::ofstream ofs(L"./log.txt", std::ofstream::out | std::ofstream::app);
    return ofs;
}

std::string Log::getDateTime()
{
    const int MAX_LEN = 200;
    char buffer[MAX_LEN];
    if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0, "HH':'mm':'ss", buffer, MAX_LEN) == 0)
        return "Error in NowTime()";

    char result[100] = { 0 };
    static DWORD first = GetTickCount();
    sprintf_s(result, "%s.%03ld", buffer, (long)(GetTickCount() - first) % 1000);
    return result;
}

