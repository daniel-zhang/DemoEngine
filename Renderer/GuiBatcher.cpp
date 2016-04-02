#include "GuiBatcher.h"
#include "REnderer.h"
#include "FontLibrary.h"

void GuiBatcher::initialize()
{
    // TODO font cache initialize
    mFontLibrary = Renderer::getInstance()->getFontLibrary().get();
}

void GuiBatcher::batch(const VisualElementGroup& inElementGroup, RenderBatchList& outRenderBatchList)
{
    // Clear previous layer->key->batch data
    mLayers.clear();

    // Process incoming visual elements
    const std::vector<VisualElement>& elements = inElementGroup.Elements;
    for (auto element : elements)
    {
        addVisualElement(element);
    }

    // Commit internal batches and caches to RenderBatchList
    outRenderBatchList.clear();
    for (uint32_t layerId = 0; layerId < mLayers.size(); ++layerId)
    {
        TKeyBatchMap& keyBatchMap = mLayers[layerId];
        for (auto iter : keyBatchMap)
        {
            const BatchKey& key     = iter.first;
            const Batch& val        = iter.second;
            const BatchCache& cache = mCachePool.getCacheByIndex(val.CacheIndex);

            // Merge information from key and batch into render batch
            GuiRenderBatch grb;
            grb.Texture         = key.Texture;
            grb.ShaderParameter = key.ShaderParamter;
            grb.ShaderType      = key.ShaderType;
            grb.DrawFlag        = key.DrawFlag;
            grb.Topology        = key.Topology;

            grb.NumElementsInBatch = val.NumElementsInBatch;
            grb.VertexNum          = cache.VertexCache.size();
            grb.IndexNum           = cache.IndexCache.size();
            grb.VertexOffset       = outRenderBatchList.VertexStream.size();
            grb.IndexOffset        = outRenderBatchList.IndexStream.size();

            outRenderBatchList.RenderBatches.push_back(grb);

            // Append batch cache data to render batch
            outRenderBatchList.VertexStream.reserve(outRenderBatchList.VertexStream.size() + cache.VertexCache.size());
            outRenderBatchList.VertexStream.insert( outRenderBatchList.VertexStream.end(), cache.VertexCache.begin(), cache.VertexCache.end());

            outRenderBatchList.IndexStream.reserve(outRenderBatchList.IndexStream.size() + cache.IndexCache.size());
            outRenderBatchList.IndexStream.insert(outRenderBatchList.IndexStream.end(), cache.IndexCache.begin(), cache.IndexCache.end());

            // Recycle batch cache
            mCachePool.recycleCache(val.CacheIndex);
        }
    }
}

GuiBatcher::Batch& GuiBatcher::getBatch(uint32_t inLayerId, const BatchKey& inKey)
{
    // Grow layers array if needed
    if (inLayerId >= mLayers.size())
    {
        mLayers.resize(inLayerId + 1);
    }
    
    TKeyBatchMap& keyBatchMap = mLayers[inLayerId];

    auto result = keyBatchMap.emplace(inKey, Batch());

    Batch& batch = result.first->second;

    // This implies new batch inserted, now allocate a cache for the newly created batch
    if (result.second == true)
    {
        batch.CacheIndex = mCachePool.allocateCache();
    }

    return batch;
}

void GuiBatcher::addVisualElement(const VisualElement& inElement)
{
    switch (inElement.ElementType)
    {
    case VisualElement::TextElement:
        addTextElement(inElement);
        break;

    case VisualElement::QuadElement:
        addQuadElement(inElement);
        break;

    case VisualElement::BorderElement:
        addBorderElement(inElement);
        break;

    case VisualElement::LineElement:
        addLineElement(inElement);
        break;

    case VisualElement::SplineElement:
        addSplineElement(inElement);
        break;

    case VisualElement::GradientElement:
        addGradientElement(inElement);
        break;

    default:
        break;
    }
}

void GuiBatcher::addQuadElement(const VisualElement& inElement)
{
    BatchKey key;
    key.Texture    = inElement.Tex;
    key.ShaderType = (key.Texture == nullptr ? EGuiShaderType::GST_DefaultShader : EGuiShaderType::GST_TextureShader);
    key.Topology   = EGuiPrimitiveTopology::GPT_TriangleList;
    key.DrawFlag   = EGuiDrawFlag::GDF_None;
    if (inElement.GammaCorrection == false)
        key.DrawFlag |= EGuiDrawFlag::GDF_NoGammaCorrection;
    if (inElement.DisabledEffect == true)
        key.DrawFlag |= EGuiDrawFlag::GDF_Disabled;

    Batch& batch = getBatch(inElement.LayerId, key);
    BatchCache& cache = mCachePool.getCacheByIndex(batch.CacheIndex);

    batch.NumElementsInBatch++;

    // Construct vertex attributes
    float width          = inElement.PixelSize.x;
    float height         = inElement.PixelSize.y;
    Vector2D topLeft     = inElement.PixelPosition;
    Vector2D topRight    = Vector2D(topLeft.x + width, topLeft.y);
    Vector2D bottomLeft  = Vector2D(topLeft.x, topLeft.y + height);
    Vector2D bottomRight = Vector2D(topLeft.x + width, topLeft.y + height);

    // Populate cache
    uint32_t indexStart    = cache.VertexCache.size();
    const Vector2D& tiling = Vector2D(1.f, 1.f);
    const Vector4D& clip   = inElement.ClipRect;
    const Color& tint      = inElement.Color;
    cache.VertexCache.push_back( GuiVertex(topLeft,     Vector2D(0.0f, 0.0f), tiling, tint, clip) );
    cache.VertexCache.push_back( GuiVertex(topRight,    Vector2D(1.0f, 0.0f), tiling, tint, clip) );
    cache.VertexCache.push_back( GuiVertex(bottomRight, Vector2D(1.0f, 1.0f), tiling, tint, clip) );
    cache.VertexCache.push_back( GuiVertex(bottomLeft,  Vector2D(0.0f, 1.0f), tiling, tint, clip) );

    if (mFrontFaceCounterClockwise)
    {
        cache.IndexCache.push_back(indexStart + 2);
        cache.IndexCache.push_back(indexStart + 1);
        cache.IndexCache.push_back(indexStart + 0);

        cache.IndexCache.push_back(indexStart + 2);
        cache.IndexCache.push_back(indexStart + 0);
        cache.IndexCache.push_back(indexStart + 3);
    }
    else
    {
        cache.IndexCache.push_back(indexStart + 2);
        cache.IndexCache.push_back(indexStart + 0);
        cache.IndexCache.push_back(indexStart + 1);

        cache.IndexCache.push_back(indexStart + 2);
        cache.IndexCache.push_back(indexStart + 3);
        cache.IndexCache.push_back(indexStart + 0);
    }
}

void GuiBatcher::addTextElement(const VisualElement& inElement)
{
    // Stop early
    if (inElement.TextAttr.Text.empty()) return;

    FontDescription fontDesc;
    /*
    fontDesc.FontName = inElement.FontName;
    fontDesc.FontSize = inElement.FontSize;
    */
    fontDesc.FontName = inElement.TextAttr.FontName;
    fontDesc.FontSize = inElement.TextAttr.FontSize;

    // Grab an arbitrary char to figure out global metrics of this font
    CharacterMetrics charMetrics;
    mFontLibrary->getMetrics(L'X', fontDesc, charMetrics);

    // Figure out the start position of text rendering
    float posX = inElement.PixelPosition.x;
    float posY = inElement.PixelPosition.y + charMetrics.GlobalMaxHeight;

    // Construct batch key for font element
    BatchKey key;
    key.ShaderType = EGuiShaderType::GST_FontShader;
    key.Topology   = EGuiPrimitiveTopology::GPT_TriangleList;
    key.Texture    = nullptr;
    key.DrawFlag = EGuiDrawFlag::GDF_None;
    if (inElement.GammaCorrection == false)
        key.DrawFlag |= EGuiDrawFlag::GDF_NoGammaCorrection;
    if (inElement.DisabledEffect == true)
        key.DrawFlag |= EGuiDrawFlag::GDF_Disabled;

    
    // Now batch each and every character of the incoming text
    Batch* pBatch      = nullptr;
    BatchCache* pCache = nullptr;
    bool fontHasKerning = mFontLibrary->hasKerning(fontDesc);
    wchar_t lastChar = 0;

    for (uint32_t i = 0; i < inElement.TextAttr.Text.size(); ++ i)
    {
        wchar_t ch = inElement.TextAttr.Text[i];

        mFontLibrary->getMetrics(ch, fontDesc, charMetrics);

        // For space/nbsp, just move posX
        bool isSpace = ( ch == L' ' || ch == 160 );
        if (!isSpace)
        {
            // Only get another batch/cache when this char references a different texture (minimize map finding)
            if (charMetrics.FontTexture != key.Texture)
            {
                key.Texture = charMetrics.FontTexture;
                pBatch = &getBatch(inElement.LayerId, key);
                pCache = &mCachePool.getCacheByIndex(pBatch->CacheIndex);
            }

            // Update batch stats
            pBatch->NumElementsInBatch++;

            if (fontHasKerning)
            {
                posX += (float)mFontLibrary->getKerning(fontDesc, lastChar, ch);
            }

            Vector2D topLeft    (posX + charMetrics.XOffset,            posY - charMetrics.YOffset);
            Vector2D topRight   (topLeft.x + charMetrics.PixelWidth,    topLeft.y);
            Vector2D bottomLeft (topLeft.x,                             topLeft.y + charMetrics.PixelHeight);
            Vector2D bottomRight(topRight.x,                            topRight.y + charMetrics.PixelHeight);

            uint32_t indexStart    = pCache->VertexCache.size();
            const Vector2D& tiling = Vector2D(1.f, 1.f);
            const Color& tint      = inElement.Color;
            const Vector4D& clip   = inElement.ClipRect;
            pCache->VertexCache.push_back(GuiVertex(topLeft,     Vector2D(charMetrics.UStart, charMetrics.VStart),                                         tiling, tint, clip));
            pCache->VertexCache.push_back(GuiVertex(topRight,    Vector2D(charMetrics.UStart + charMetrics.USize, charMetrics.VStart),                     tiling, tint, clip));
            pCache->VertexCache.push_back(GuiVertex(bottomRight, Vector2D(charMetrics.UStart + charMetrics.USize, charMetrics.VStart + charMetrics.VSize), tiling, tint, clip));
            pCache->VertexCache.push_back(GuiVertex(bottomLeft,  Vector2D(charMetrics.UStart, charMetrics.VStart + charMetrics.VSize),                     tiling, tint, clip));

            if (mFrontFaceCounterClockwise)
            {
                pCache->IndexCache.push_back(indexStart + 2);
                pCache->IndexCache.push_back(indexStart + 1);
                pCache->IndexCache.push_back(indexStart + 0);

                pCache->IndexCache.push_back(indexStart + 2);
                pCache->IndexCache.push_back(indexStart + 0);
                pCache->IndexCache.push_back(indexStart + 3);
            }
            else
            {
                pCache->IndexCache.push_back(indexStart + 2);
                pCache->IndexCache.push_back(indexStart + 0);
                pCache->IndexCache.push_back(indexStart + 1);

                pCache->IndexCache.push_back(indexStart + 2);
                pCache->IndexCache.push_back(indexStart + 3);
                pCache->IndexCache.push_back(indexStart + 0);
            }
        }

        // Step posX
        posX += charMetrics.XAdvance;
        // Record this char for later kerning
        lastChar = ch;
    }
}

void GuiBatcher::addBorderElement(const VisualElement& inElement)
{
    // Texture dimension in texels
    uint32_t textureWidth  = inElement.Tex->getWidth();
    uint32_t textureHeight = inElement.Tex->getHeight();

    //    ___LeftX          ___RightPaddedX
    //   /   _LeftPaddedX  /  ____RightX
    //  /   /             /  /
    //  +--+-------------+--+ <---TopY
    //  |  |c1           |c2| 
    //  +--o-------------o--+ <---TopPaddedY
    //  |  |             |  |
    //  |  |c3           |c4|
    //  +--o-------------o--+ <---BottomPaddedY
    //  |  |             |  | 
    //  +--+-------------+--+ <---BottomY

    // Figure out the element's pixel rect in screen space 
    float leftX   = inElement.PixelPosition.x;
    float rightX  = inElement.PixelPosition.x + inElement.PixelSize.x;
    float topY    = inElement.PixelPosition.y;
    float bottomY = inElement.PixelPosition.y + inElement.PixelSize.y;

    // Figure out the padded pixel rect in screen space
    // The paddings are given as normalized data, which complies with UV coordinates
    float leftPaddedX   = leftX + textureWidth * inElement.BorderAttr.Padding.x;
    float topPaddedY    = topY + textureHeight * inElement.BorderAttr.Padding.y;
    float rightPaddedX  = rightX - textureWidth * inElement.BorderAttr.Padding.z;
    float bottomPaddedY = bottomY - textureHeight * inElement.BorderAttr.Padding.w;

    // Validate padded rect
    if (rightPaddedX < leftPaddedX)
        leftPaddedX = rightPaddedX = leftX + inElement.PixelSize.x/2;
    if (bottomPaddedY < topPaddedY)
        bottomPaddedY = topPaddedY = topY + inElement.PixelSize.y/2;

    //    ___LeftU          ___RightPaddedU
    //   /   _LeftPaddedU  /  ____RightU
    //  /   /             /  /
    //  +--+-------------+--+ <---TopV
    //  |  |c1           |c2| 
    //  +--o-------------o--+ <---TopPaddedV
    //  |  |             |  |
    //  |  |c3           |c4|
    //  +--o-------------o--+ <---BottomPaddedV
    //  |  |             |  | 
    //  +--+-------------+--+ <---BottomV

    // Figure out the texture rect in texture space. No support for texture atlas since we are tiling.
    float leftU   = 0.f;
    float rightU  = 1.f;
    float topV    = 0.f;
    float bottomV = 1.f;

    // Figure out the padded texture rect in texture space. 
    float leftPaddedU   = inElement.BorderAttr.Padding.x;
    float topPaddedV    = inElement.BorderAttr.Padding.y;
    float rightPaddedU  = rightU - inElement.BorderAttr.Padding.z;
    float bottomPaddedV = bottomV - inElement.BorderAttr.Padding.w;

    // Compute tiling(tile = pixels/texels)
    float tilingU = (rightPaddedX - leftPaddedX) / (textureWidth * (rightPaddedU - leftPaddedU));
    float tilingV = (bottomPaddedY - topPaddedY) / (textureHeight * (bottomPaddedV - topPaddedV));
    tilingU = tilingU < 1.f ? 1.f : tilingU;
    tilingV = tilingV < 1.f ? 1.f : tilingV;

    // Find a proper batch to which geometry data is going to be added.
    BatchKey key;
    key.Texture        = inElement.Tex;
    // The padded UV rect must be passed into shader to allow for non-standard tiling
    key.ShaderParamter = Vector4D(leftPaddedU, topPaddedV, rightPaddedU, bottomPaddedV);
    key.ShaderType     = EGuiShaderType::GST_BorderShader;
    key.Topology       = EGuiPrimitiveTopology::GPT_TriangleList;
    key.DrawFlag = EGuiDrawFlag::GDF_None;
    if (inElement.GammaCorrection == false)
        key.DrawFlag |= EGuiDrawFlag::GDF_NoGammaCorrection;
    if (inElement.DisabledEffect == true)
        key.DrawFlag |= EGuiDrawFlag::GDF_Disabled;


    Batch& batch = getBatch(inElement.LayerId, key);
    batch.NumElementsInBatch += 8;

    BatchCache& cache          = mCachePool.getCacheByIndex(batch.CacheIndex);
    std::vector<GuiVertex>& vc = cache.VertexCache;
    std::vector<uint32_t>& ic  = cache.IndexCache;
    const Color& tint           = inElement.Color;
    const Vector4D& clip       = inElement.ClipRect;
    uint32_t indexStart        = vc.size();

    // TopLeft corner, no tiling
    vc.push_back( GuiVertex( Vector2D(leftX, topY),              Vector2D(leftU, topV),               Vector2D(1.f, 1.f),     tint, clip) ); // TL vertex 0
    vc.push_back( GuiVertex( Vector2D(leftPaddedX, topY),        Vector2D(leftPaddedU, topV),         Vector2D(1.f, 1.f),     tint, clip) ); // TR vertex 1
    vc.push_back( GuiVertex( Vector2D(leftPaddedX, topPaddedY),  Vector2D(leftPaddedU, topPaddedV),   Vector2D(1.f, 1.f),     tint, clip) ); // BR vertex 2
    vc.push_back( GuiVertex( Vector2D(leftX, topPaddedY),        Vector2D(leftU, topPaddedV),         Vector2D(1.f, 1.f),     tint, clip) ); // BL vertex 3

    // Top border, tile along U
    vc.push_back( GuiVertex( Vector2D(leftPaddedX, topY),        Vector2D(leftPaddedU, topV),         Vector2D(tilingU, 1.f), tint, clip) ); // TL vertex 4
    vc.push_back( GuiVertex( Vector2D(rightPaddedX,topY),        Vector2D(rightPaddedU,topV),         Vector2D(tilingU, 1.f), tint, clip) ); // TR vertex 5
    vc.push_back( GuiVertex( Vector2D(rightPaddedX,topPaddedY),  Vector2D(rightPaddedU, topPaddedV),  Vector2D(tilingU, 1.f), tint, clip) ); // BR vertex 6
    vc.push_back( GuiVertex( Vector2D(leftPaddedX, topPaddedY),  Vector2D(leftPaddedU, topPaddedV),   Vector2D(tilingU, 1.f), tint, clip) ); // BL vertex 7

    // TopRight corner, no tiling
    vc.push_back( GuiVertex( Vector2D(rightPaddedX, topY),       Vector2D(rightPaddedU, topV),        Vector2D(1.f, 1.f),     tint, clip) ); // TL vertex 8
    vc.push_back( GuiVertex( Vector2D(rightX,topY),              Vector2D(rightU,topV),               Vector2D(1.f, 1.f),     tint, clip) ); // TR vertex 9
    vc.push_back( GuiVertex( Vector2D(rightX, topPaddedY),       Vector2D(rightU, topPaddedV),        Vector2D(1.f, 1.f),     tint, clip) ); // BR vertex 10
    vc.push_back( GuiVertex( Vector2D(rightPaddedX, topPaddedY), Vector2D(rightPaddedU, topPaddedV),  Vector2D(1.f, 1.f),     tint, clip) ); // BL vertex 11

    // CenterLeft border, tile along V
    vc.push_back( GuiVertex( Vector2D(leftX, topPaddedY),        Vector2D(leftU, topPaddedV),         Vector2D(1.f, tilingV), tint, clip) ); // TL vertex 12 
    vc.push_back( GuiVertex( Vector2D(leftPaddedX,topPaddedY),   Vector2D(leftPaddedU,topPaddedV),    Vector2D(1.f, tilingV), tint, clip) ); // TR vertex 13
    vc.push_back( GuiVertex( Vector2D(leftPaddedX,bottomPaddedY),Vector2D(leftPaddedU, bottomPaddedV),Vector2D(1.f, tilingV), tint, clip) ); // BR vertex 14
    vc.push_back( GuiVertex( Vector2D(leftX, bottomPaddedY),     Vector2D(leftU, bottomPaddedV),      Vector2D(1.f, tilingV), tint, clip) ); // BL vertex 15

    // Center: N/A

    // CenterRight border, tile along V
    vc.push_back( GuiVertex( Vector2D(rightPaddedX, topPaddedY),  Vector2D(rightPaddedU, topPaddedV),   Vector2D(1.f, tilingV), tint, clip) ); // TL vertex 16 
    vc.push_back( GuiVertex( Vector2D(rightX,topPaddedY),         Vector2D(rightU,topPaddedV),          Vector2D(1.f, tilingV), tint, clip) ); // TR vertex 17
    vc.push_back( GuiVertex( Vector2D(rightX,bottomPaddedY),      Vector2D(rightU, bottomPaddedV),      Vector2D(1.f, tilingV), tint, clip) ); // BR vertex 18
    vc.push_back( GuiVertex( Vector2D(rightPaddedX,bottomPaddedY),Vector2D(rightPaddedU, bottomPaddedV),Vector2D(1.f, tilingV), tint, clip) ); // BL vertex 19

    // BottomLeft corner, no tiling
    vc.push_back( GuiVertex( Vector2D(leftX, bottomPaddedY),      Vector2D(leftU, bottomPaddedV),      Vector2D(1.f, 1.f),      tint, clip) ); // TL vertex 20
    vc.push_back( GuiVertex( Vector2D(leftPaddedX, bottomPaddedY),Vector2D(leftPaddedU, bottomPaddedV),Vector2D(1.f, 1.f),      tint, clip) ); // TR vertex 21
    vc.push_back( GuiVertex( Vector2D(leftPaddedX, bottomY),      Vector2D(leftPaddedU, bottomV),      Vector2D(1.f, 1.f),      tint, clip) ); // BR vertex 22
    vc.push_back( GuiVertex( Vector2D(leftX, bottomY),            Vector2D(leftU, bottomV),            Vector2D(1.f, 1.f),      tint, clip) ); // BL vertex 23
    
    // Bottom border, tile along U
    vc.push_back(GuiVertex(Vector2D(leftPaddedX, bottomPaddedY),  Vector2D(leftPaddedU, bottomPaddedV), Vector2D(tilingU, 1.f), tint, clip)); // TL vertex 24
    vc.push_back(GuiVertex(Vector2D(rightPaddedX, bottomPaddedY), Vector2D(rightPaddedU,bottomPaddedV), Vector2D(tilingU, 1.f), tint, clip)); // TR vertex 25
    vc.push_back(GuiVertex(Vector2D(rightPaddedX, bottomY),       Vector2D(rightPaddedU, bottomV),      Vector2D(tilingU, 1.f), tint, clip)); // BR vertex 26
    vc.push_back(GuiVertex(Vector2D(leftPaddedX, bottomY),        Vector2D(leftPaddedU, bottomV),       Vector2D(tilingU, 1.f), tint, clip)); // BL vertex 27

    // BottomRight corner, no tiling
    vc.push_back(GuiVertex(Vector2D(rightPaddedX, bottomPaddedY), Vector2D(rightPaddedU, bottomPaddedV),Vector2D(1.f, 1.f),     tint, clip)); // TL vertex 28
    vc.push_back( GuiVertex( Vector2D(rightX, bottomPaddedY),     Vector2D(rightU, bottomPaddedV),      Vector2D(1.f, 1.f),     tint, clip) ); // TR vertex 29
    vc.push_back( GuiVertex( Vector2D(rightX, bottomY),           Vector2D(rightU, bottomV),            Vector2D(1.f, 1.f),     tint, clip) ); // BR vertex 30
    vc.push_back( GuiVertex( Vector2D(rightPaddedX, bottomY),     Vector2D(rightPaddedU, bottomV),      Vector2D(1.f, 1.f),     tint, clip) ); // BL vertex 31 

    uint32_t TL = 0;
    uint32_t TR = 1;
    uint32_t BR = 2;
    uint32_t BL = 3;
    for (uint32_t i = 0; i < 8; ++i)
    {
        if (mFrontFaceCounterClockwise)
        {
            ic.push_back(indexStart + TL);
            ic.push_back(indexStart + BR);
            ic.push_back(indexStart + TR);

            ic.push_back(indexStart + TL);
            ic.push_back(indexStart + BL);
            ic.push_back(indexStart + BR);
        }
        else
        {
            ic.push_back(indexStart + TL);
            ic.push_back(indexStart + TR);
            ic.push_back(indexStart + BR);

            ic.push_back(indexStart + TL);
            ic.push_back(indexStart + BR);
            ic.push_back(indexStart + BL);
        }
        TL += 4;
        TR += 4;
        BL += 4;
        BR += 4;
    }
}

static bool lineIntersect( const Vector2D& P1, const Vector2D& P2, const Vector2D& P3, const Vector2D& P4, 
    Vector2D& outIntersection, float Tolerance = .1f)
{
    float NumA = ((P4.x - P3.x)*(P1.y - P3.y) - (P4.y - P3.y)*(P1.x - P3.x));
    float NumB = ((P2.x - P1.x)*(P1.y - P3.y) - (P2.y - P1.y)*(P1.x - P3.x));

    float Denom = (P4.y - P3.y)*(P2.x - P1.x) - (P4.x - P3.x)*(P2.y - P1.y);

    if (Math::isNearlyZero(NumA) && Math::isNearlyZero(NumB))
    {
        // Lines are the same
        outIntersection = (P1 + P2) / 2;
        return true;
    }

    if (Math::isNearlyZero(Denom))
    {
        // Lines are parallel
        return false;
    }

    float B = NumB / Denom;
    float A = NumA / Denom;

    // Note that this is a "tweaked" intersection test for the purpose of joining line segments.  We don't just want to know if the line segements
    // Intersect, but where they would if they don't currently. Except that we dont care in the case that where the segments 
    // intersection is so far away that its infeasible to use the intersection point later.
    if (A >= -Tolerance && A <= (1.0f + Tolerance) && B >= -Tolerance && B <= (1.0f + Tolerance))
    {
        outIntersection = P1 + (P2 - P1)*A;
        return true;
    }

    return false;
}

// On pre-filtered lines http://people.csail.mit.edu/ericchan/articles/prefilter/
void GuiBatcher::addLineElement(const VisualElement& inElement)
{
    if (inElement.LineAttr.Points.size() < 2)
        return;

    // Anti-aliased line
    if (inElement.LineAttr.Prefiltered)
    {
        const float radius = 1.5f;
        //const float radius = 1.f;
        const float requestedThickness = max(1.f, inElement.LineAttr.Thickness);
        const float thickness          = std::ceil((2.f * radius + requestedThickness) * std::sqrt(2.f));;
        //const float thickness =  2.f * radius + requestedThickness;
        const float halfThickness      = radius + thickness * .5f;

        // Construct key
        BatchKey key;
        key.Texture = nullptr;
        //key.ShaderParamter = Vector4D(requestedThickness, radius, 0, 0);
        key.ShaderParamter = Vector4D(inElement.LineAttr.Thickness, radius, 0, 0);
        key.ShaderType     = EGuiShaderType::GST_LineShader;
        key.Topology       = EGuiPrimitiveTopology::GPT_TriangleList;
        key.DrawFlag       = EGuiDrawFlag::GDF_None;
        if (inElement.GammaCorrection == false)
            key.DrawFlag |= EGuiDrawFlag::GDF_NoGammaCorrection;
        if (inElement.DisabledEffect == true)
            key.DrawFlag |= EGuiDrawFlag::GDF_Disabled;


        // Find batch
        Batch& batch = getBatch(inElement.LayerId, key);
        batch.NumElementsInBatch += inElement.LineAttr.Points.size();
        BatchCache& cache           = mCachePool.getCacheByIndex(batch.CacheIndex);
        std::vector<GuiVertex>& vc  = cache.VertexCache;
        std::vector<uint32_t>& ic   = cache.IndexCache;
        const Color& tint           = inElement.Color;
        const Vector4D& clip        = inElement.ClipRect;

        // Fill up batch
        const std::vector<Vector2D>& points = inElement.LineAttr.Points;
        const Vector2D& posInScreenSpace = inElement.PixelPosition;

        Vector2D start  = posInScreenSpace + points[0];
        Vector2D end    = posInScreenSpace + points[1];
        Vector2D normal = Vector2D(start.y - end.y, end.x - start.x).normalize();
        Vector2D up     = normal * halfThickness;

        vc.push_back(GuiVertex(start + up, start, end, tint, clip));
        vc.push_back(GuiVertex(start - up, start, end, tint, clip));

        for (uint32_t i = 1; i < points.size(); ++ i)
        {
            end = posInScreenSpace + points[i];

            uint32_t indexStart = vc.size();

            normal = Vector2D(start.y - end.y, end.x - start.x).normalize();
            up     = normal * halfThickness;

            Vector2D intersectUpper = end + up;
            Vector2D intersectLower = end - up;
            Vector2D intersectCenter = end;

            // Validate next points
            if (i + 1 < points.size())
            {
                // The end point of the next segment
                const Vector2D nextEnd = posInScreenSpace + points[i + 1];

                const Vector2D nextUp = Vector2D(end.y - nextEnd.y, nextEnd.x - end.x).normalize() * halfThickness;

                Vector2D intersectionPoint;
                if (lineIntersect(start + up, end + up, end + nextUp, nextEnd + nextUp, intersectionPoint))
                {
                    // If the lines intersect adjust where the line starts
                    intersectUpper = intersectionPoint;
                }

                if (lineIntersect(start - up, end - up, end - nextUp, nextEnd - nextUp, intersectionPoint))
                {
                    // If the lines intersect adjust where the line starts
                    intersectLower = intersectionPoint;
                }
                intersectCenter = (intersectUpper + intersectLower) * .5f;
            }

            if (i > 1)
            {
                // Make a copy of the last two vertices and update their start and end position to reflect the new line segment
                GuiVertex startV1 = vc[indexStart - 1];
                GuiVertex startV2 = vc[indexStart - 2];

                startV1.TexCoords.x = start.x;
                startV1.TexCoords.y = start.y;
                startV1.TexCoords.z = intersectCenter.x;
                startV1.TexCoords.w = intersectCenter.y;

                startV2.TexCoords.x = start.x;
                startV2.TexCoords.y = start.y;
                startV2.TexCoords.z = intersectCenter.x;
                startV2.TexCoords.w = intersectCenter.y;

                indexStart += 2;
                vc.push_back(startV2);
                vc.push_back(startV1);
            }

            vc.push_back(GuiVertex(intersectUpper, start, intersectCenter, tint, clip));
            vc.push_back(GuiVertex(intersectLower, start, intersectCenter, tint, clip));

            if (mFrontFaceCounterClockwise)
            {
                ic.push_back(indexStart - 1);
                ic.push_back(indexStart + 0);
                ic.push_back(indexStart - 2);

                ic.push_back(indexStart + 0);
                ic.push_back(indexStart - 1);
                ic.push_back(indexStart + 1);
            }
            else
            {
                ic.push_back(indexStart - 1);
                ic.push_back(indexStart - 2);
                ic.push_back(indexStart + 0);

                ic.push_back(indexStart + 0);
                ic.push_back(indexStart + 1);
                ic.push_back(indexStart - 1);
            }

            start = end;
        }
    }
    // Simple line
    else
    {
        BatchKey key;
        key.Texture        = nullptr;
        key.ShaderType     = EGuiShaderType::GST_DefaultShader;
        key.Topology       = EGuiPrimitiveTopology::GPT_LineList;
        key.DrawFlag       = EGuiDrawFlag::GDF_None;
        if (inElement.GammaCorrection == false)
            key.DrawFlag |= EGuiDrawFlag::GDF_NoGammaCorrection;
        if (inElement.DisabledEffect == true)
            key.DrawFlag |= EGuiDrawFlag::GDF_Disabled;


        Batch& batch = getBatch(inElement.LayerId, key);

        batch.NumElementsInBatch += inElement.LineAttr.Points.size();

        BatchCache& cache          = mCachePool.getCacheByIndex(batch.CacheIndex);
        std::vector<GuiVertex>& vc = cache.VertexCache;
        std::vector<uint32_t>& ic  = cache.IndexCache;
        const Color& tint          = inElement.Color;
        const Vector4D& clip       = inElement.ClipRect;

        for (uint32_t i = 0; i < inElement.LineAttr.Points.size() - 1; ++i)
        {
            uint32_t indexStart = vc.size();
            Vector2D StartPos   = inElement.PixelPosition + inElement.LineAttr.Points[i] + Vector2D(-.5f, -.5f);
            Vector2D EndPos     = inElement.PixelPosition + inElement.LineAttr.Points[i + 1] + Vector2D(-.5f, -.5f);

            vc.push_back(GuiVertex(StartPos, Vector2D(), Vector2D(), tint, clip));
            vc.push_back(GuiVertex(EndPos, Vector2D(), Vector2D(), tint, clip));

            ic.push_back(indexStart);
            ic.push_back(indexStart + 1);
        }
    }
}

void GuiBatcher::addSplineElement(const VisualElement& inElement)
{
    const float DirectLength = (inElement.SplineAttr.EndPt - inElement.SplineAttr.StartPt).length();
    const float HandleLength = ((inElement.SplineAttr.EndPt - inElement.SplineAttr.EndDir) - (inElement.SplineAttr.StartPt + inElement.SplineAttr.StartDir)).length();
    float NumSteps = Math::clamp<float>(1, 256, std::ceil(max(DirectLength, HandleLength) / 15.0f));

    // 1 is the minimum thickness we support
    float InThickness = max(1.0f, inElement.SplineAttr.Thickness);

    // The radius to use when checking the distance of pixels to the actual line.  Arbitrary value based on what looks the best
    const float Radius = 1.5f;

    // Compute the actual size of the line we need based on thickness.  Need to ensure pixels that are at least Thickness/2 + Sample radius are generated so that we have enough pixels to blend.
    const float LineThickness = std::ceil((2.0f * Radius + InThickness) * std::sqrt(2.0f));

    // The amount we increase each side of the line to generate enough pixels
    const float HalfThickness = LineThickness * .5f + Radius;

    BatchKey key;
    key.Texture        = nullptr;
    key.ShaderParamter = Vector4D(inElement.SplineAttr.Thickness, Radius, 0, 0);
    key.ShaderType     = EGuiShaderType::GST_LineShader;
    key.Topology       = EGuiPrimitiveTopology::GPT_TriangleList;
    key.DrawFlag       = EGuiDrawFlag::GDF_None;
    if (inElement.GammaCorrection == false)
        key.DrawFlag |= EGuiDrawFlag::GDF_NoGammaCorrection;
    if (inElement.DisabledEffect == true)
        key.DrawFlag |= EGuiDrawFlag::GDF_Disabled;


    Batch& batch = getBatch(inElement.LayerId, key);
    BatchCache& cache = mCachePool.getCacheByIndex(batch.CacheIndex);
    std::vector<GuiVertex>& vc = cache.VertexCache;
    std::vector<uint32_t>& ic = cache.IndexCache;
    batch.NumElementsInBatch += 1;

    const Vector2D StartPt = inElement.PixelPosition + inElement.SplineAttr.StartPt;
    const Vector2D StartDir = inElement.SplineAttr.StartDir;
    const Vector2D EndPt = inElement.PixelPosition + inElement.SplineAttr.EndPt;
    const Vector2D EndDir = inElement.SplineAttr.EndDir;

    // Compute the normal to the line
    Vector2D Normal = Vector2D(StartPt.y - EndPt.y, EndPt.x - StartPt.x).normalize();
    Vector2D Up = Normal * HalfThickness;

    // Generate the first segment
    const float Alpha = 1.0f / NumSteps;
    Vector2D StartPos = StartPt;
    Vector2D EndPos = Vector2D(Math::cubicInterp(StartPt, StartDir, EndPt, EndDir, Alpha));

    //Color FinalColor = GetElementColor(InPayload.Tint, NULL);

    vc.push_back(GuiVertex(StartPos + Up, StartPos, EndPos, inElement.Color, inElement.ClipRect));
    vc.push_back(GuiVertex(StartPos - Up, StartPos, EndPos, inElement.Color, inElement.ClipRect));

    // Generate the rest of the segments
    for (uint32_t Step = 0; Step < NumSteps; ++Step)
    {
        // Skip the first step as it was already generated
        if (Step > 0)
        {
            const float StepAlpha = (Step + 1.0f) / NumSteps;
            EndPos = Vector2D(Math::cubicInterp(StartPt, StartDir, EndPt, EndDir, StepAlpha));
        }

        uint32_t IndexStart = vc.size();

        // Compute the normal to the line
        Vector2D SegmentNormal = Vector2D(StartPos.y - EndPos.y, EndPos.x - StartPos.x).normalize();

        // Create the new vertices for the thick line segment
        Up = SegmentNormal * HalfThickness;

        vc.push_back(GuiVertex(EndPos + Up, StartPos, EndPos, inElement.Color, inElement.ClipRect));
        vc.push_back(GuiVertex(EndPos - Up, StartPos, EndPos, inElement.Color, inElement.ClipRect));

        if (mFrontFaceCounterClockwise)
        {
            ic.push_back(IndexStart - 2);
            ic.push_back(IndexStart + 0);
            ic.push_back(IndexStart - 1);

            ic.push_back(IndexStart + 0);
            ic.push_back(IndexStart - 1);
            ic.push_back(IndexStart + 1);
        }
        else
        {
            ic.push_back(IndexStart - 2);
            ic.push_back(IndexStart - 1);
            ic.push_back(IndexStart + 0);

            ic.push_back(IndexStart + 0);
            ic.push_back(IndexStart + 1);
            ic.push_back(IndexStart - 1);
        }

        StartPos = EndPos;
    }
}

void GuiBatcher::addGradientElement(const VisualElement& inElement)
{
    if (inElement.GradientAttr.Stops.empty())
        return;

    BatchKey key;
    key.Texture    = nullptr;
    key.ShaderType = EGuiShaderType::GST_DefaultShader;
    key.Topology   = EGuiPrimitiveTopology::GPT_TriangleList;
    key.DrawFlag   = EGuiDrawFlag::GDF_None;
    if (inElement.GammaCorrection == false)
        key.DrawFlag |= EGuiDrawFlag::GDF_NoGammaCorrection;
    if (inElement.DisabledEffect == true)
        key.DrawFlag |= EGuiDrawFlag::GDF_Disabled;


    Batch& batch               = getBatch(inElement.LayerId, key);
    BatchCache& cache          = mCachePool.getCacheByIndex(batch.CacheIndex);
    std::vector<GuiVertex>& vc = cache.VertexCache;
    std::vector<uint32_t>& ic  = cache.IndexCache;

    batch.NumElementsInBatch += 1;

    Vector2D topLeft     = inElement.PixelPosition;
    Vector2D topRight    = Vector2D(inElement.PixelPosition.x + inElement.PixelSize.x, inElement.PixelPosition.y);
    Vector2D bottomRight = Vector2D(inElement.PixelPosition.x + inElement.PixelSize.x, inElement.PixelPosition.y + inElement.PixelSize.y);
    Vector2D bottomLeft  = Vector2D(inElement.PixelPosition.x, inElement.PixelPosition.y + inElement.PixelSize.y);

    std::vector<GradientStop> stops = inElement.GradientAttr.Stops;

    GradientStop& firstStop = stops[0];
    GradientStop& lastStop = stops[stops.size() - 1];

    // clamp stops
    if (inElement.GradientAttr.Orientation == GradientAttributes::VerticalGradient)
    {
        if (topLeft.x < firstStop.Position.x + topLeft.x)
        {
            // The first stop is after the left side of the quad.  Add a stop at the left side of the quad using the same color as the first stop
            stops.insert(stops.begin(), GradientStop(Vector2D(0.0, 0.0), firstStop.Color));
        }

        if (topRight.x > lastStop.Position.x + topLeft.x)
        {
            // The last stop is before the right side of the quad.  Add a stop at the right side of the quad using the same color as the last stop
            stops.push_back(GradientStop(bottomRight, lastStop.Color));
        }
    }
    else
    {

        if (topLeft.y < firstStop.Position.y + topLeft.y)
        {
            // The first stop is after the top side of the quad.  Add a stop at the top side of the quad using the same color as the first stop
            stops.insert(stops.begin(), GradientStop(Vector2D(0.0, 0.0), firstStop.Color));
        }

        if (bottomLeft.y > lastStop.Position.y + topLeft.y)
        {
            // The last stop is before the bottom side of the quad.  Add a stop at the bottom side of the quad using the same color as the last stop
            stops.push_back(GradientStop(bottomRight, lastStop.Color));
        }
    }

    const Color& clr = inElement.Color;
    const Vector4D& clip = inElement.ClipRect;

    uint32_t IndexOffsetStart = ic.size();

    for (uint32_t StopIndex = 0; StopIndex < stops.size(); ++StopIndex)
    {
        uint32_t IndexStart = vc.size();

        const GradientStop& CurStop = stops[StopIndex];

        // The start vertex at this stop
        Vector2D StartPt;
        // The end vertex at this stop
        Vector2D EndPt;

        if (inElement.GradientAttr.Orientation == GradientAttributes::VerticalGradient)
        {
            // Gradient stop is vertical so gradients to left to right
            StartPt = topLeft;
            EndPt = bottomLeft;
            StartPt.x += CurStop.Position.x;
            EndPt.x += CurStop.Position.x;
        }
        else
        {
            // Gradient stop is horizontal so gradients to top to bottom
            StartPt = topLeft;
            EndPt = topRight;
            StartPt.y += CurStop.Position.y;
            EndPt.y += CurStop.Position.y;
        }

        if (StopIndex == 0)
        {
            // First stop does not have a full quad yet so do not create indices
            vc.push_back(GuiVertex(StartPt, Vector2D(), Vector2D(), CurStop.Color, clip));
            vc.push_back(GuiVertex(EndPt, Vector2D(), Vector2D(), CurStop.Color, clip));
        }
        else
        {
            // All stops after the first have indices and generate quads
            vc.push_back(GuiVertex(StartPt, Vector2D(), Vector2D(), CurStop.Color, clip));
            vc.push_back(GuiVertex(EndPt, Vector2D(), Vector2D(), CurStop.Color, clip));

            // Connect the indices to the previous vertices
            if (mFrontFaceCounterClockwise)
            {
                ic.push_back(IndexStart - 2);
                ic.push_back(IndexStart + 0);
                ic.push_back(IndexStart - 1);

                ic.push_back(IndexStart + 0);
                ic.push_back(IndexStart + 1);
                ic.push_back(IndexStart - 1);
            }
            else
            {
                ic.push_back(IndexStart - 2);
                ic.push_back(IndexStart - 1);
                ic.push_back(IndexStart + 0);

                ic.push_back(IndexStart + 0);
                ic.push_back(IndexStart - 1);
                ic.push_back(IndexStart + 1);
            }
        }
    }
}




