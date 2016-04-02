#pragma once

#include "CommonHeaders.h"

class Path
{
public:
    Path(){}

    Path(const std::wstring& dir)
    {
        setPathString(dir);
    }

    void setPathString(const std::wstring& dir)
    {
        parseStringPath(dir);
    }

    bool isEmpty()
    {
        return mPathEntries.empty();
    }

    bool isValid() const
    {

    }

    bool isAbsolute() const
    {
        if (!mPathEntries.empty())
        {
            return (mPathEntries[0].find(L':') != std::wstring::npos);
        }
        return false;
    }

    void toString(std::wstring& str) const
    {
        for (auto item : mPathEntries)
        {
            str += item + L'/';
        }
    }

    std::wstring toString() const
    {
        std::wstring str;
        toString(str);
        return str;
    }

    Path& navigateUp(uint32_t numLevel = 1)
    {
        for (uint32_t i = 0; i < numLevel; ++i)
        {
            mPathEntries.pop_back();
        }
        return *this;
    }

    void append(const Path& other) 
    {
        mPathEntries.insert(mPathEntries.end(), other.mPathEntries.begin(), other.mPathEntries.end());
    }

private:
    void parseStringPath(const std::wstring& path);

    bool isDelimeter(const wchar_t& ch);

    std::size_t findDelimetersFrom(const std::wstring& path, std::size_t posFrom );

private:
    const static wchar_t mDelimters[];
    const static uint8_t mNumDelimter;

    std::vector<std::wstring> mPathEntries;
};

class FileDescriptor
{
public:
    void setName(const std::wstring& name_ext);

    void setDir(const Path& inDir)
    {
        mDir = inDir;
    }

    const std::wstring& getName() const
    {
        return mName;
    }

    const std::wstring& getNakedName() const
    {
        return mNakedName;
    }

    const std::wstring& getExt() const
    {
        return mExt;
    }

    std::wstring getPathName() const
    {
        return mDir.toString() + L"/" + mName;
    }

    const Path& getDir() const
    {
        return mDir;
    }

private:
    std::wstring mName;      // name_ext
    std::wstring mNakedName; // name without ext
    std::wstring mExt;

    Path mDir;
};


class FileList
{
public:
    bool isEmpty() const
    {
        return mFileList.empty();
    }

    uint32_t getNum() const
    {
        return mFileList.size();
    }

    void add(const FileDescriptor& inFileDesc)
    {
        mFileList.push_back(inFileDesc);
    }

    const std::vector<FileDescriptor>& getList()
    {
        return mFileList;
    }

    void append(const FileList& other)
    {
        mFileList.insert(this->mFileList.end(), other.mFileList.begin(), other.mFileList.end());
    }

private:
    std::vector<FileDescriptor> mFileList;
};

class PathList
{
public:
    bool isEmpty() const;

    uint32_t getNum() const;

    void add(const Path& inDirectory)
    {
        mPathList.push_back(inDirectory);
    }

private:
    std::vector<Path> mPathList;
};

class FileOp
{
public:
    static void setCurrPath(const Path& dir);
    static Path getCurrPath();

    static void listFiles(const Path& inDir, const std::wstring& extFilter, FileList& outFileList);
    static FileList listFiles(const Path& inDir, const std::wstring& extFilter);

    static void listPaths(const Path& inDir, PathList& outDirList);
    static PathList listPaths(const Path& inDir);

    static const Path& getExePath();

    enum EResourceType
    {
        ShaderResource = 0,
        MeshResource,
        AnimationResource,
        FontResource,
        TextureResource,
        DebugDump,
    };

    template<EResourceType RT>
    static const Path& getDefaultPath()
    {
        switch (RT)
        {

        case FileOp::ShaderResource:
            {
                static Path path(getExePath().toString() + L"../Shader");
                return path;
            }

        case FileOp::MeshResource:
        case FileOp::AnimationResource:
            {
                static Path path(getExePath().toString() + L"../Resource");
                return path;
            }

        case FileOp::FontResource:
            {
                static Path path(getExePath().toString() + L"../Resource/Font");
                return path;
            }

        case FileOp::TextureResource:
            {
                static Path path(getExePath().toString() + L"../Resource/Texture");
                return path;
            }

        case FileOp::DebugDump:
            {
                static Path path(getExePath().toString() + L"../Resource/DebugDump");
                return path;
            }

        default:
            // Just to suppress the warning
            {
                static Path path(getExePath().toString() + L"../Shader");
                return path;
            }
        }
    }

    static bool checkFileExists(const std::wstring& filename );

    static bool checkFileExists(const std::string& filename );

    static void readFile(const std::string& filename, std::string& output);

    static void readFile(const std::wstring& filename, std::wstring& output);

    static void parseFilename(const std::string& filename, std::string& outNakedName, std::string& outExt);

    static void parseFilename(const std::wstring& filename, std::wstring& outNakedName, std::wstring& outExt);

};