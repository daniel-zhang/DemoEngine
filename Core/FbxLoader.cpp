#include "FbxLoader.h"

class FbxLoaderImple
{
public:
    FbxLoaderImple(): mFbxMgr(nullptr), mFbxScene(nullptr) { }
    ~FbxLoaderImple() { destroy(); }

    void initialize()
    {
        if (mFbxMgr)
        {
            mFbxMgr->Destroy();
            mFbxMgr = nullptr;
        }

        mFbxMgr = FbxManager::Create();
        FbxIOSettings* ios = FbxIOSettings::Create(mFbxMgr, IOSROOT);
        mFbxMgr->SetIOSettings(ios);
        mFbxScene = FbxScene::Create(mFbxMgr, "ImportedScene");
    }

    void destroy()
    {
        if (mFbxScene)
        {
            mFbxScene->Clear();
        }
        if (mFbxMgr)
        {
            mFbxMgr->Destroy();
            mFbxMgr = nullptr;
        }
    }

    void load(const FbxInput& input, FbxOutput& output)
    {
        FileOp::parseFilename(input.Filename, output.SceneName, std::string());
        std::string fullPathName = input.FilePath + "/" + input.Filename;

        FbxImporter* importer = FbxImporter::Create(mFbxMgr, "");
        bool succeeded = importer->Initialize(fullPathName.c_str(), -1, mFbxMgr->GetIOSettings());
        if (!succeeded)
        { 
            output.addError("Failed to initialize FbxImporter.");
            return;
        }

        mFbxScene->Clear();
        importer->Import(mFbxScene);

        std::string sceneName = mFbxScene->GetName();
        std::string initName = mFbxScene->GetInitialName();

        importer->Destroy();

        output.OutputScene = new FbxHelper::Scene(mFbxScene);
        //FbxHelper::Scene helperScene(mFbxScene);
    }


protected:
    FbxManager* mFbxMgr;
    FbxScene* mFbxScene;
};


/************************************************************************/
/* Wrapper class                                                                     */
/************************************************************************/
FbxLoader::FbxLoader() : pImple(nullptr)
{
    pImple = new FbxLoaderImple();
}

FbxLoader::~FbxLoader()
{
    if (pImple)
    {
        delete pImple;
        pImple = nullptr;
    }
}

void FbxLoader::initialize() { pImple->initialize(); }

void FbxLoader::destroy() { pImple->destroy(); }

void FbxLoader::load(const FbxInput& input, FbxOutput& output)
{
    pImple->load(input, output);
}
