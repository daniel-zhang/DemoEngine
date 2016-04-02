#pragma once

#include "CommonHeaders.h"

//
// A simple logging class 
//
class Log
{
public:
    enum ELogLevel { error, warning, info, debug };

    Log(){}

    ~Log();

    std::ostringstream& getLineBuffer(ELogLevel level);

    static void setPath(const std::string& inPath, bool append = true);

protected:
    std::string getDateTime();

    static std::ofstream& getFileStream();

protected:
    std::ostringstream mLineBuffer;
};

#define LOG(level) Log().getLineBuffer(Log::##level)


