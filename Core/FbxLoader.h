#pragma once

#include "Util.h"
#include "FbxHelper.h"

class FbxInput
{
public:
    std::string FilePath;
    std::string Filename;
    bool RemoveDuplicatedVertex;
};

class FbxOutput
{
public:
    FbxOutput():OutputScene(nullptr){}
    ~FbxOutput()
    {
        if (OutputScene)
        {
            delete OutputScene;
            OutputScene = nullptr;
        }
    }

public:
    std::string SceneName;
    std::vector< std::string> Errors;
    bool IsSucceeded;

    FbxHelper::Scene* OutputScene;

public:
    void addError(const char* e)
    {
        Errors.push_back(e);
        IsSucceeded = false;
    }
};

class FbxLoader
{
public:
    static FbxLoader* getInstance()
    {
        static FbxLoader instance;
        return &instance;
    }

    ~FbxLoader();

public:
    void initialize();
    void destroy();

    void load(const FbxInput& input, FbxOutput& output);

protected:
    FbxLoader();
    FbxLoader(const FbxLoader&);
    FbxLoader& operator=(const FbxLoader&);

protected:
    class FbxLoaderImple* pImple;
};