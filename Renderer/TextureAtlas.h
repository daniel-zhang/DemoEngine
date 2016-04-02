#pragma once

#include "RendererCommon.h"
//#include "Texture.h"
#include "Texture.h"

struct AtlasRect
{
    AtlasRect(){ memset(this, 0, sizeof AtlasRect); }

    AtlasRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, Texture* inTexture) :
        X(x), Y(y), Width(w), Height(h), AtlasTexture(inTexture)
    {}

    //DynamicTexture2D* AtlasTexture;
    Texture* AtlasTexture;

    // X-Right, Y-Down, UpperLeft-Origin coordinate
    uint32_t X;
    uint32_t Y;
    uint32_t Width;
    uint32_t Height;
};

class TextureAtlas
{
public:
    TextureAtlas();

    ~TextureAtlas() {}

    // @param inBitmap: Client code must make sure the format of inBitmap is compatible with the format of TextureAtlas
    // This update the CPU shadow caches without committing changes to GPU.
    void feedBitmap(uint8_t* inBitmap, uint32_t inWidth, uint32_t inHeight, AtlasRect& outRect);

    // Commit texture changes(made after last commit)to GPU 
    void commit();

    void setFormat(DXGI_FORMAT format) { mDefaultFormat = format; }

    void setTextureDimension(uint32_t width, uint32_t height){ mDefaultTextureWidth = width; mDefaultTextureHeight = height; }

    inline uint32_t getTextureNum() const { return mForest.size(); }

    uint32_t getMemFootprint() const;

    void debugDumpTextures() const;

protected:
    struct AtlasNode
    {
        AtlasNode(uint32_t x, uint32_t y, uint32_t w, uint32_t h) : 
            X(x), Y(y), Width(w), Height(h), Left(nullptr), Right(nullptr)
        {}

        ~AtlasNode()
        {
            safe_delete(Left);
            safe_delete(Right);
        }

        AtlasNode* Left; 
        AtlasNode* Right;

        uint32_t X, Y;
        uint32_t Width, Height;
    };

    class AtlasBinaryTree
    {
    public:
        AtlasNode* Root;
        //DynamicTexture2D* BackingTexture;
        Texture* BackingTexture;

    public:
        AtlasBinaryTree(uint32_t width, uint32_t height, DXGI_FORMAT format);
        
        ~AtlasBinaryTree();
        AtlasBinaryTree(const AtlasBinaryTree&) = delete;
        AtlasBinaryTree& operator=(const AtlasBinaryTree&) = delete;

        AtlasNode* allocateNode(uint32_t width, uint32_t height) { return _allocateNode(Root, width, height); }

    protected:
        AtlasNode* _allocateNode(AtlasNode* start, uint32_t targetW, uint32_t targetH);
    };

protected:
    DXGI_FORMAT mDefaultFormat;
    uint32_t mDefaultTextureWidth;
    uint32_t mDefaultTextureHeight;

    std::vector<std::unique_ptr<AtlasBinaryTree> > mForest;
};