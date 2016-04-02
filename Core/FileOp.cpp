#include "FileOp.h"

const wchar_t Path::mDelimters[] = {'/', '\\'};

void Path::parseStringPath(const std::wstring& path)
{
    mPathEntries.clear();

    std::size_t left = 0;
    std::size_t right = findDelimetersFrom(path, left);
    while (right != std::wstring::npos)
    {
        // Extract directory item
        std::wstring pathItem = path.substr(left, right - left);
        //vcheck(dirItem.empty() == false, L"Should Never Be Empty");

        if (pathItem.empty())
        {
            // Abandon empty string
        }
        else if (isDelimeter(pathItem[0]))
        {
            if (pathItem.size() > 1)
            {
                // Strip the heading '/'
                mPathEntries.push_back(pathItem.substr(1));
            }
            else
            {
                // Abandon this single '/'
            }
        }
        else
        {
            mPathEntries.push_back(pathItem);
        }

        // Move extract window
        left = right + 1;
        right = findDelimetersFrom(path, left);
    }
    // Capture the tail when last slash is not present like in "head:/body/tail"
    if (left < path.size())
    {
        mPathEntries.push_back(path.substr(left));
    }
}

bool Path::isDelimeter(const wchar_t& ch)
{
    for (uint32_t i = 0; i < mNumDelimter; ++i)
    {
        if (ch == mDelimters[i])
            return true;
    }
    return false;
}

std::size_t Path::findDelimetersFrom(const std::wstring& path, std::size_t posFrom)
{
    std::size_t targetPos = path.size();
    for (uint32_t i = 0; i < mNumDelimter; ++i)
    {
        std::size_t tmpPos = path.find(mDelimters[i], posFrom);
        if (tmpPos != std::wstring::npos && tmpPos < targetPos)
        {
            targetPos = tmpPos;
        }
    }
    return (targetPos == path.size()) ? std::wstring::npos : targetPos;
}

const uint8_t Path::mNumDelimter = 2;


Path FileOp::getCurrPath()
{
    static DWORD defaultBufferSize = MAX_PATH;
    /*
    DWORD WINAPI GetCurrentDirectory(
    _In_  DWORD  nBufferLength,
    _Out_ LPTSTR lpBuffer
    );
    */
    wchar_t* buffer = new wchar_t[defaultBufferSize];
    memset(buffer, 0, defaultBufferSize);

    DWORD actualSize = GetCurrentDirectoryW(defaultBufferSize, buffer);

    check(actualSize != 0);

    if (actualSize > defaultBufferSize)
    {
        if (buffer) delete[] buffer;

        buffer = new wchar_t[actualSize];
        memset(buffer, 0, actualSize);

        actualSize = GetCurrentDirectoryW(actualSize, buffer);

        check(actualSize != 0);
    }

    Path dir;

    dir.setPathString(std::wstring(buffer));

    if (buffer)
    {
        delete[] buffer;
    }

    return dir;
}

//#pragma comment(lib, "User32.lib")

void FileOp::listFiles(const Path& inDir, const std::wstring& extFilter, FileList& outFileList)
{
    std::wstring searchString; 
    inDir.toString(searchString);
    searchString += L"\\*." + extFilter;

    WIN32_FIND_DATA wfd;
    HANDLE hFind = FindFirstFile(searchString.c_str(), &wfd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        // Empty
        FindClose(hFind);
        return;
    }
    do
    {
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            int debug = 1;
            //_tprintf(TEXT("  %s   <DIR>\n"), wfd.cFileName);
        }
        else
        {
            LARGE_INTEGER filesize;
            filesize.LowPart = wfd.nFileSizeLow;
            filesize.HighPart = wfd.nFileSizeHigh;

            FileDescriptor fileDesc;
            fileDesc.setName(wfd.cFileName);
            fileDesc.setDir(inDir);

            outFileList.add(fileDesc);
        }
    } 
    while (FindNextFile(hFind, &wfd) != 0);

    DWORD dwError = 0;
    dwError = GetLastError();
    check(dwError == ERROR_NO_MORE_FILES);

    FindClose(hFind);
}


FileList FileOp::listFiles(const Path& inDir, const std::wstring& extFilter)
{
    FileList files;
    listFiles(inDir, extFilter, files);
    return files;
}

void FileOp::listPaths(const Path& inDir, PathList& outDirList)
{
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA wfd;

    std::wstring baseDirStr = inDir.toString();

    std::wstring searchDirStr = baseDirStr + L"\\*.*";
    hFind = FindFirstFile(searchDirStr.c_str(), &wfd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        DWORD dwError = 0;
        dwError = GetLastError();

        FindClose(hFind);
        return;
    }
    do
    {
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            Path targetDir;
            targetDir.setPathString(baseDirStr + L"\\" + wfd.cFileName);
            outDirList.add(targetDir);
        }
    } while (FindNextFile(hFind, &wfd) != 0);

    DWORD dwError = 0;
    dwError = GetLastError();
    check(dwError == ERROR_NO_MORE_FILES);

    FindClose(hFind);
}

PathList FileOp::listPaths(const Path& inDir)
{
    PathList dl;
    listPaths(inDir, dl);
    return dl;
}

void FileOp::setCurrPath(const Path& path)
{
    std::wstring pathStr;
    path.toString(pathStr);

    SetCurrentDirectoryW(pathStr.c_str());
}

const Path& FileOp::getExePath()
{
    static Path exePath;
    if (exePath.isEmpty())
    {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(GetModuleHandleW(NULL), path, MAX_PATH);
        exePath.setPathString(path);
        exePath.navigateUp(); // Abandon the last item, which is the name of exe
    }
    return exePath;
}

bool FileOp::checkFileExists(const std::wstring& filename)
{
    WIN32_FIND_DATAW ffd;
    HANDLE handle = FindFirstFileW(filename.c_str(), &ffd);
    if (handle != INVALID_HANDLE_VALUE)
    {
        FindClose(handle);
        return true;
    }
    return false;
}

bool FileOp::checkFileExists(const std::string& filename)
{
    WIN32_FIND_DATAA ffd;
    HANDLE handle = FindFirstFileA(filename.c_str(), &ffd);
    if (handle != INVALID_HANDLE_VALUE)
    {
        FindClose(handle);
        return true;
    }
    return false;
}

void FileOp::readFile(const std::string& filename, std::string& output)
{
    std::ifstream ifs(filename, std::ifstream::in);

    check(ifs.good());

    ifs.seekg(0, std::ios::end);
    size_t size = static_cast<size_t>(ifs.tellg());
    ifs.seekg(0);

    output.resize(size);
    ifs.read(&output[0], size);
}

void FileOp::readFile(const std::wstring& filename, std::wstring& output)
{
    std::wifstream wifs(filename, std::wifstream::in);

    check(wifs.good());

    wifs.seekg(0, std::ios::end);
    size_t size = static_cast<size_t>(wifs.tellg());
    wifs.seekg(0);

    output.resize(size);
    wifs.read(&output[0], size);
}

void FileOp::parseFilename(const std::string& filename, std::string& outNakedName, std::string& outExt)
{
    std::size_t posLastDot = filename.rfind('.');
    if (filename.empty())
    {
        return;
    }
    else if (posLastDot != std::string::npos)
    {
        if (filename.compare(".") == 0)
        {
            outNakedName = ".";
            outExt.clear();
        }
        else
        {
            outNakedName = filename.substr(0, posLastDot);
            outExt = filename.substr(posLastDot, std::wstring::npos);
        }
    }
    else
    {
        outNakedName = filename;
        outExt.clear();
    }
}

void FileOp::parseFilename(const std::wstring& filename, std::wstring& outNakedName, std::wstring& outExt)
{
    std::size_t posLastDot = filename.rfind(L'.');
    if (filename.empty())
    {
        return;
    }
    else if (posLastDot != std::string::npos)
    {
        if (filename.compare(L".") == 0)
        {
            outNakedName = L".";
            outExt.clear();
        }
        else
        {
            outNakedName = filename.substr(0, posLastDot);
            outExt = filename.substr(posLastDot, std::wstring::npos);
        }
    }
    else
    {
        outNakedName = filename;
        outExt.clear();
    }
}


void FileDescriptor::setName(const std::wstring& name_ext)
{
    mName = name_ext;
    FileOp::parseFilename(mName, mNakedName, mExt);
}
