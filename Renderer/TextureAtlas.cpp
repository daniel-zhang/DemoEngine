#include "TextureAtlas.h"

// This works on two prerequisites:
// 1. dst and src image formats are compatible(i.e. have same stride and can be directly copied)
// 2. The rectangle into dstImg is big enough to hold srcImg. This should be taken care of by atlas algorithm.
/*
void copyBitmap(Image2D& dstImg, uint32_t dstX, uint32_t dstY, uint8_t* srcImg, uint32_t srcWidth, uint32_t srcHeight)
{
    std::vector<uint8_t>& rawMem = dstImg.getRawData();

    uint32_t dstRowPitch = dstImg.getRowPitch();
    uint32_t stride      = dstRowPitch / dstImg.getWidth();
    uint32_t srcRowPitch = stride * srcWidth;

    // Copy by rows
    for (uint32_t srcRow = 0; srcRow < srcHeight; ++srcRow)
    {
        uint32_t dstRow = dstY + srcRow;
        uint32_t dstCol = dstX;
        void* dstRowOffset = &rawMem[dstRow * dstRowPitch + dstCol * stride];
        void* srcRowOffset = &srcImg[srcRow * srcRowPitch];
        memcpy(dstRowOffset, srcRowOffset, srcRowPitch);
    }
}
*/
void copyBitmap(Image& dstImg, uint32_t dstX, uint32_t dstY, uint8_t* srcSurface, uint32_t srcWidth, uint32_t srcHeight)
{
    std::vector<uint8_t>& dstSurface = dstImg.getSurface();

    uint32_t dstRowPitch = dstImg.getRowPitch();
    uint32_t stride      = dstRowPitch / dstImg.getWidth();
    uint32_t srcRowPitch = stride * srcWidth;

    // Copy by rows
    for (uint32_t srcRow = 0; srcRow < srcHeight; ++srcRow)
    {
        uint32_t dstRow = dstY + srcRow;
        uint32_t dstCol = dstX;
        void* dstRowOffset = &dstSurface[dstRow * dstRowPitch + dstCol * stride];
        void* srcRowOffset = &srcSurface[srcRow * srcRowPitch];
        memcpy(dstRowOffset, srcRowOffset, srcRowPitch);
    }
}

TextureAtlas::TextureAtlas() :
mDefaultTextureWidth(1024),
mDefaultTextureHeight(1024),
mDefaultFormat(DXGI_FORMAT_A8_UNORM) 
{ }

void TextureAtlas::feedBitmap(uint8_t* inBitmap, uint32_t inWidth, uint32_t inHeight, AtlasRect& outRect)
{
    AtlasNode* targetNode = nullptr;

    //
    // Try to allocate a proper rect in the current forest for the incoming bitmap 
    //
    uint32_t debugSize = mForest.size();
    for (uint32_t i = 0; i < mForest.size(); ++i)
    {
        std::unique_ptr<AtlasBinaryTree>& tree = mForest[i];
        targetNode = tree->allocateNode(inWidth, inHeight);
        if (targetNode)
        {
            // Update backing texture's image cache
            Image& imgCache = tree->BackingTexture->updateImageCache();
            copyBitmap(imgCache, targetNode->X, targetNode->Y, inBitmap, inWidth, inHeight);
            //Image2D& imageCache = tree->BackingTexture->update();
            //copyBitmap(imageCache, targetNode->X, targetNode->Y, inBitmap, inWidth, inHeight);
            
            // Populate outRect 
            outRect.X            = targetNode->X;
            outRect.Y            = targetNode->Y;
            outRect.Width        = targetNode->Width;
            outRect.Height       = targetNode->Height;
            outRect.AtlasTexture = tree->BackingTexture;
            return;
        }
    }

    //
    // The incoming bitmap does not fit into any existing textures, let's create a new one for it. (i.e. a new tree in the forest) 
    //

    // Grow dimensions if the defaults are too small
    uint32_t w = max(mDefaultTextureWidth, 2 * inWidth);
    uint32_t h = max(mDefaultTextureHeight, 2 * inHeight);

    mForest.emplace_back(new AtlasBinaryTree(w, h, mDefaultFormat) ) ;
    std::unique_ptr<AtlasBinaryTree>& tree = mForest[mForest.size() - 1];

    // This one must succeed
    targetNode = tree->allocateNode(inWidth, inHeight);
    check(targetNode != nullptr);
    {
        // Update backing texture's image cache
        /*
        Image2D& imageCache = tree->BackingTexture->update();
        copyBitmap(imageCache, targetNode->X, targetNode->Y, inBitmap, inWidth, inHeight);
        */
        Image& imgCache = tree->BackingTexture->updateImageCache();
        copyBitmap(imgCache, targetNode->X, targetNode->Y, inBitmap, inWidth, inHeight);

        // Populate outRect 
        outRect.X            = targetNode->X;
        outRect.Y            = targetNode->Y;
        outRect.Width        = targetNode->Width;
        outRect.Height       = targetNode->Height;
        outRect.AtlasTexture = tree->BackingTexture;
    }
    return;
}

void TextureAtlas::commit()
{
    // Commit dirty textures to GPU
    for (std::unique_ptr<AtlasBinaryTree>& tree : mForest)
    {
        tree->BackingTexture->commitImageCache_Tex2D();
    }
}

uint32_t TextureAtlas::getMemFootprint() const
{
    uint32_t memUsage = 0;
    for (const std::unique_ptr< AtlasBinaryTree>& tree : mForest)
    {
        memUsage += tree->BackingTexture->getMemoryUsage();
    }
    return memUsage;
}

void TextureAtlas::debugDumpTextures() const
{
    const Path& path = FileOp::getDefaultPath<FileOp::DebugDump>();

    for (uint32_t i = 0; i < mForest.size(); ++i)
    {
        const std::unique_ptr<AtlasBinaryTree>& tree = mForest[i];
        std::wstring file_path_name = path.toString() + wstring_format(L"tex_%d", i);

        tree->BackingTexture->debugSave(file_path_name, Image::EImageFileFormat::PNG);
    }
}

TextureAtlas::AtlasBinaryTree::AtlasBinaryTree(uint32_t width, uint32_t height, DXGI_FORMAT format) 
{

    BackingTexture = new Texture;
    BackingTexture->initializeAsDynamic(format, width, height);
    //BackingTexture = new  DynamicTexture2D(width, height, format);
    //BackingTexture->create();
    Root = new AtlasNode(0, 0, width, height);
}

TextureAtlas::AtlasBinaryTree::~AtlasBinaryTree()
{
    safe_delete(Root);
    safe_delete(BackingTexture);
}

TextureAtlas::AtlasNode* TextureAtlas::AtlasBinaryTree::_allocateNode(AtlasNode* start, uint32_t targetW, uint32_t targetH)
{
    // Traverse down to leaf nodes in such a manner that smaller bitmaps prefer to occupy smaller rectangles
    if (start->Left || start->Right)
    {
        AtlasNode* newNode = nullptr;

        if (start->Left)
        {
            newNode = _allocateNode(start->Left, targetW, targetH);
            if (newNode) return newNode;
        }

        if (start->Right)
        {
            newNode = _allocateNode(start->Right, targetW, targetH);
            if (newNode) return newNode;
        }

        // Not enough space
        return nullptr;
    }

    // Now 'start' is a leaf node, shrink it to fit the incoming rect, then split its remaining space into 2 child nodes.
    if (targetW > start->Width || targetH > start->Height)
        return nullptr; // Not enough space

    // Split guarantees that:
    // 1) left node has fewer space than right node 
    // 2) the ratio of allocated rectangles should be as close to 1 as possible
    uint32_t remainingWidth = start->Width - targetW;
    uint32_t remainingHeight = start->Height - targetH;

    if (remainingHeight < remainingWidth)
    {
        // Split vertically
        start->Left = new AtlasNode(start->X, start->Y + targetH, targetW, remainingHeight);
        start->Right = new AtlasNode(start->X + targetW, start->Y, remainingWidth, start->Height);
    }
    else
    {
        // Split horizontally 
        start->Left = new AtlasNode(start->X + targetW, start->Y, remainingWidth, targetH);
        start->Right = new AtlasNode(start->X, start->Y + targetH, start->Width, remainingHeight);
    }
    start->Width = targetW;
    start->Height = targetH;

    return start;
}
