#pragma once
#include "Core/Util.h"
#include "VisualElementGroups.h"
#include "VertexDeclaration.h"
//#include "Texture.h"
#include "Texture.h"

enum class EGuiShaderType
{
    GST_DefaultShader,
    GST_FontShader,
    GST_TextureShader,
    GST_BorderShader,
    GST_LineShader,
};

enum EGuiDrawFlag
{
    GDF_None               = 0,
    GDF_Disabled           = 1 << 0,
    GDF_NoGammaCorrection  = 1 << 1,
    GDF_IgnoreTextureAlpha = 1 << 2,
    GDF_NoBlending         = 1 << 3,
};

enum class EGuiPrimitiveTopology
{
    GPT_LineList,
    GPT_TriangleList,
};

struct GuiRenderBatch
{
    Texture* Texture;
    Vector4D ShaderParameter;
    EGuiShaderType ShaderType;
    EGuiPrimitiveTopology Topology;
    uint32_t DrawFlag;

    uint32_t NumElementsInBatch; 

    uint32_t VertexOffset;
    uint32_t VertexNum;
    uint32_t IndexOffset;
    uint32_t IndexNum;
};

struct RenderBatchList
{
    std::vector<GuiRenderBatch> RenderBatches;
    std::vector<GuiVertex> VertexStream;
    std::vector<uint32_t> IndexStream;

    void clear()
    {
        RenderBatches.clear();
        VertexStream.clear();
        IndexStream.clear();
    }
};

class GuiBatcher
{
public:
    GuiBatcher() : mFontLibrary(nullptr), mFrontFaceCounterClockwise(false){}

    inline void setFrontFaceCounterClockwise(bool flag){ mFrontFaceCounterClockwise = flag; }

    void initialize();

    void batch(const VisualElementGroup& inElementGroup, RenderBatchList& outGuiDrawBuffer);

protected:
    void addVisualElement(const VisualElement& inElement);

    void addQuadElement(const VisualElement& inElement);

    void addTextElement(const VisualElement& inElement);

    void addBorderElement(const VisualElement& inElement);

    void addLineElement(const VisualElement& inElement);

    void addSplineElement(const VisualElement& inElement);

    void addGradientElement(const VisualElement& inElement);

protected:
    bool mFrontFaceCounterClockwise;

protected:
    struct BatchKey
    {
        Texture* Texture;
        Vector4D ShaderParamter;
        EGuiShaderType ShaderType;
        EGuiPrimitiveTopology Topology;
        uint32_t DrawFlag;
    };

    struct KeyEqual
    {
        bool operator()(const BatchKey& lhs, const BatchKey&rhs) const
        {
            return
                lhs.Texture == rhs.Texture &&
                lhs.ShaderType == rhs.ShaderType &&
                lhs.DrawFlag == rhs.DrawFlag &&
                lhs.Topology == rhs.Topology &&
                lhs.ShaderParamter.x == rhs.ShaderParamter.x &&
                lhs.ShaderParamter.y == rhs.ShaderParamter.y &&
                lhs.ShaderParamter.z == rhs.ShaderParamter.z &&
                lhs.ShaderParamter.w == rhs.ShaderParamter.w;
        }
    };

    struct KeyHash
    {
        size_t operator()(const BatchKey& key)const
        {
            size_t val = 0;
            MurmurHash3_x86_32((const void*)&key, sizeof BatchKey, 0, (void*)&val);
            return val;
        }
    };

    struct Batch
    {
        Batch():NumElementsInBatch(0), CacheIndex(0){}

        uint32_t NumElementsInBatch; // Keep track of how many visual elements are added to this batch

        uint32_t CacheIndex;
    };

    typedef std::unordered_map<BatchKey, Batch, KeyHash, KeyEqual> TKeyBatchMap;
    typedef std::vector<TKeyBatchMap> TLayers;
    TLayers mLayers;

    Batch& getBatch(uint32_t inLayerId, const BatchKey& inKey);

    class FontLibrary* mFontLibrary;
    
protected:
    struct BatchCache
    {
        std::vector<GuiVertex> VertexCache;
        std::vector<uint32_t> IndexCache;
        void clear()
        {
            // As long as the BatchCache object lives(which is guaranteed by CachePool), memory attached to its vectors will not 
            // be released by vector::clear(), effectively avoid reallocation.
            VertexCache.clear();
            IndexCache.clear();
        }
    };

    class CachePool
    {
    public:
        BatchCache& getCacheByIndex(uint32_t index)
        {
            return mCaches[index];
        }

        uint32_t allocateCache()
        {
            uint32_t cacheIndex = 0;
            if (mFreeCacheIndex.empty())
            {
                mCaches.push_back(BatchCache());
                cacheIndex = mCaches.size() - 1;
            }
            else
            {
                cacheIndex = mFreeCacheIndex[mFreeCacheIndex.size() - 1];
                mFreeCacheIndex.pop_back();
            }

            return cacheIndex;
        }

        void recycleCache(uint32_t index)
        {
            // Clear the cache for next reuse
            mCaches[index].clear();
            mFreeCacheIndex.push_back(index);
        }

    protected:
        std::vector<BatchCache> mCaches;
        std::vector<uint32_t> mFreeCacheIndex;
    };

    CachePool mCachePool;
};
