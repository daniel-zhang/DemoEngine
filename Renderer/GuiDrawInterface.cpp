#include "GuiDrawInterface.h"
#include "Renderer.h"
#include "Prototype_ShaderTemplate.h"

void generateConvolutionTable(Texture& inTex)
{
    Image img;
    /*
    img.createAsEmptyImage(DXGI_FORMAT_A8_UNORM, 32, 32);
    std::vector<uint8_t>& table = img.getSurface();

    // Use sine to approximate ...
    uint32_t entryNum = img.getWidth();
    for (uint32_t i = 0; i < entryNum; ++i)
    {
        float step = Math::HALF_PI * (float)i / entryNum;
        table[i] = (uint8_t)std::trunc(256 * std::sin(step));
    }

    inTex.initializeAsImmutableFromImage(img);
    */

    const Path& dir = FileOp::getDefaultPath<FileOp::TextureResource>();
    std::wstring filterFilename = dir.toString() + L"filter_box.tga";
    
    img.createFromFile(filterFilename);
    inTex.initializeAsImmutableFromImage(img);
}

GuiDrawInterface::GuiDrawInterface() : 
    mDev(nullptr), mCtx(nullptr), 
    mRenderStates(nullptr),
    //mVertexBuffer(nullptr), mIndexBuffer(nullptr),
    mInitialVertexBufferByteSize(4096), 
    mInitialIndexBufferByteSize(4096) ,
    mShowDebugClipping(false)
{
}

void GuiDrawInterface::initialize()
{
    if (mDev && mCtx) return;

    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();

    mBatcher.initialize();

    // Create render states if not created yet
    mRenderStates = RenderStates::getInstance();
    mSampler = Sampler::getInstance();

    mGuiVS.initialize();
    mGuiPS.initialize();

    // Create vertex and index buffers
    mVertexBuffer.initialize(GeometryBuffer::DynamicVertexBuffer, mInitialVertexBufferByteSize);
    mIndexBuffer.initialize(GeometryBuffer::DynamicIndexBuffer, mInitialIndexBufferByteSize);

    // Load anti-aliased line filters from disk, then create a backup one on the fly
    const Path& dir = FileOp::getDefaultPath<FileOp::TextureResource>();
    std::wstring boxFilter = dir.toString() + L"filter_box.tga";
    std::wstring gaussFilter = dir.toString() + L"filter_gauss.tga";
    if (FileOp::checkFileExists(boxFilter))
    {
        Image img;
        img.createFromFile(boxFilter);
        mBoxFilter.initializeAsImmutableFromImage(img);
    }
    if (FileOp::checkFileExists(gaussFilter))
    {
        Image img;
        img.createFromFile(gaussFilter);
        mGaussFilter.initializeAsImmutableFromImage(img);
    }
    {
        uint32_t numFilterEntry = 32;
        Image img;
        img.createAsEmptyImage(DXGI_FORMAT_R8_UINT, numFilterEntry, 1);
        std::vector<uint8_t>& table = img.getSurface();

        // Use sine to approximate ...
        for (uint32_t i = 0; i < numFilterEntry; ++i)
        {
            float step = Math::HALF_PI * (float)i / numFilterEntry;
            table[i] = (uint8_t)std::trunc(256 * std::sin(step));
        }
        mBackupFilter.initializeAsImmutableFromImage(img);
    }
}

void GuiDrawInterface::destroy()
{
    mVertexBuffer.destroy();
    mIndexBuffer.destroy();
    mGuiVS.destroy();
    mGuiPS.destroy();
    mShowDebugClipping = false;

    mDev = nullptr;
    mCtx = nullptr;
}

bool GuiDrawInterface::createRenderTarget(HWND hwnd, const RenderTargetSpec& inRTSpec)
{
    auto result = mHwndViewportMap.emplace(hwnd, GuiViewport());
    if (result.second == true)
    {
        GuiViewport& gvp = result.first->second;
        gvp.create(hwnd, inRTSpec);
        return true;
    }
    else
    {
        vcheck(false, L"Recreate render target with same Id");
        return false;
    }
}

bool GuiDrawInterface::removeRenderTarget(HWND hwnd)
{
    auto result = mHwndViewportMap.find(hwnd);
    if (result != mHwndViewportMap.end())
    {
        mHwndViewportMap.erase(result);
        return true;
    }
    return false;
}

void GuiDrawInterface::resizeRenderTarget(HWND hwnd, const Vector2D& newSize)
{
    auto result = mHwndViewportMap.find(hwnd);
    if (result != mHwndViewportMap.end())
    {
        GuiViewport& gvp = result->second;
        gvp.resize(hwnd, newSize);
    }
}

void GuiDrawInterface::draw(const VisualElementGroups& inGroups)
{
    const std::vector<VisualElementGroup>& groups = inGroups.getContent();

    // Disable depth test before drawing any Gui to ensure back-to-front drawing order
    mRenderStates->disableDepthTest();

    // Save the current blend state
    ID3D11BlendState* tempBlendState; 
    float tempFactor[4]; 
    uint32_t tempMask;
    mCtx->OMGetBlendState(&tempBlendState, tempFactor, &tempMask);

    // Enable blending 
    mCtx->OMSetBlendState(mRenderStates->BlendState_On, 0, 0xFFFFFFFF);

    // Batch and draw each group
    for (const VisualElementGroup& group : groups)
    {
        // Batch this group of visual elements
        mBatcher.batch(group, mGuiRenderBatchList);

        // Stop early if nothing to draw
        if (mGuiRenderBatchList.RenderBatches.empty())
            continue;

        // Find render target by hwnd of this group
        auto result = mHwndViewportMap.find(group.Hwnd);
        check(result != mHwndViewportMap.end());
        GuiViewport& gvp = result->second;

        // Set hwnd render target to pipeline
        gvp.D3DRenderTarget->clear(Palette::QGray_Light);
        gvp.D3DRenderTarget->bind();
        gvp.D3DViewport.bind();

        // Make sure local VB is large enough 
        uint32_t vertexBytes = mGuiRenderBatchList.VertexStream.size() * sizeof GuiVertex;
        if (mVertexBuffer.getBufferByteSize() < vertexBytes)
            mVertexBuffer.resize(vertexBytes);

        // Copy vertex data from generated render batch list to local VB
        memcpy(mVertexBuffer.lock(), &mGuiRenderBatchList.VertexStream[0], vertexBytes);
        mVertexBuffer.unlock();

        // Set local VB to pipeline
        uint32_t stride = sizeof GuiVertex;
        uint32_t offset = 0;
        mCtx->IASetVertexBuffers(0, 1, mVertexBuffer.getResourcePtr(), &stride, &offset);

        // Make sure local IB has enough room
        uint32_t indexBytes = mGuiRenderBatchList.IndexStream.size() * sizeof uint32_t;
        if (mIndexBuffer.getBufferByteSize() < indexBytes)
            mIndexBuffer.resize(indexBytes);
 
        // Copy index data from generated render batch list to local IB
        memcpy(mIndexBuffer.lock(), &mGuiRenderBatchList.IndexStream[0], indexBytes);
        mIndexBuffer.unlock();

        // Set local IB to pipeline
        mCtx->IASetIndexBuffer(mIndexBuffer.getResource(), DXGI_FORMAT_R32_UINT, 0);

        // Iterate through the render batch list to draw
        for (const GuiRenderBatch& renderBatch : mGuiRenderBatchList.RenderBatches)
        {
            switch (renderBatch.Topology)
            {
            case EGuiPrimitiveTopology::GPT_LineList:
                mRenderStates->setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
                break;
            case EGuiPrimitiveTopology::GPT_TriangleList:
                mRenderStates->setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                break;
            }

            /*
            mDefaultVS->setParameter(ECbufferSemantics::ViewMatrix, Matrix());
            mDefaultVS->setParameter(ECbufferSemantics::ProjMatrix, gvp.ProjMatrix);

            mDefaultVS->apply();
            */
            mGuiVS.setParameter(GuiVertexShader::ECBufferParam::ViewMatrix, Matrix());
            mGuiVS.setParameter(GuiVertexShader::ECBufferParam::ProjMatrix, gvp.ProjMatrix);
            mGuiVS.apply();

            if (renderBatch.DrawFlag & EGuiDrawFlag::GDF_Disabled)
                mGuiPS.enableDisabledEffect();
            else
                mGuiPS.disableDisabledEffect();

            if (renderBatch.DrawFlag & EGuiDrawFlag::GDF_NoGammaCorrection)
            {
                /// TODO
            }
            if (!mShowDebugClipping)
                mGuiPS.disableDebugClip();
            else
                mGuiPS.enableDebugClip();
            

            switch (renderBatch.ShaderType)
            {
            case EGuiShaderType::GST_DefaultShader:
                mGuiPS.setNoTextureShader();
                mGuiPS.setParameter(GuiPixelShaderDSL::ESamplerParam::GuiSampler, mSampler->Sampler_MinMagMipLinear);

                break;

            case EGuiShaderType::GST_LineShader:
                mGuiPS.setLineShader();
                mGuiPS.setParameter(GuiPixelShaderDSL::ETextureParam::LineFilter, &mBoxFilter);
                mGuiPS.setParameter(GuiPixelShaderDSL::ESamplerParam::GuiSampler, mSampler->Sampler_MinMagMipLinear);
                mGuiPS.setParameter(GuiPixelShaderDSL::ECBufferParam::GuiMiscParam, renderBatch.ShaderParameter);
                break;

            case EGuiShaderType::GST_BorderShader:
                mGuiPS.setBorderShader();
                mGuiPS.setParameter(GuiPixelShaderDSL::ETextureParam::GuiTexture, renderBatch.Texture);
                mGuiPS.setParameter(GuiPixelShaderDSL::ESamplerParam::GuiSampler, mSampler->Sampler_MinMagMipPoint_Tiling);
                mGuiPS.setParameter(GuiPixelShaderDSL::ECBufferParam::GuiMiscParam, renderBatch.ShaderParameter);

                break;

            case EGuiShaderType::GST_FontShader:
                mGuiPS.setFontShader();
                mGuiPS.setParameter(GuiPixelShaderDSL::ETextureParam::GuiTexture, renderBatch.Texture);
                mGuiPS.setParameter(GuiPixelShaderDSL::ESamplerParam::GuiSampler, mSampler->Sampler_MinMagMipPoint);
                break;

            case EGuiShaderType::GST_TextureShader:
                mGuiPS.setDefaultShader();
                mGuiPS.setParameter(GuiPixelShaderDSL::ETextureParam::GuiTexture, renderBatch.Texture);
                mGuiPS.setParameter(GuiPixelShaderDSL::ESamplerParam::GuiSampler, mSampler->Sampler_Anisotropic);

                break;
            default:
                break;
            }

            mGuiPS.apply();

            // Issue draw call
            mCtx->DrawIndexed(renderBatch.IndexNum, renderBatch.IndexOffset, renderBatch.VertexOffset);
        }

        gvp.D3DRenderTarget->present();
    }

    // Enable depth test after Gui drawing is finished
    mRenderStates->enableDepthTest();

    // Restore blend state after Gui drawing is finished
    mCtx->OMSetBlendState(tempBlendState, tempFactor, tempMask);
    safe_release(tempBlendState);
}

GuiDrawInterface::GuiViewport::~GuiViewport()
{
    if (D3DRenderTarget)
    {
        D3DRenderTarget->destroy();
        delete D3DRenderTarget;

        D3DViewport.destroy();
    }
}

void GuiDrawInterface::GuiViewport::create(HWND hwnd, const RenderTargetSpec& inSpec)
{
    if (D3DRenderTarget)
        return;

    // Create render target
    D3DRenderTarget = new RenderTarget(hwnd, inSpec);
    D3DRenderTarget->create();

    // Figure out the actual size
    uint32_t width = 0;
    uint32_t height = 0;

    if (inSpec.FillHwndWindow)
    {
        RECT clientArea;
        ::GetClientRect(hwnd, &clientArea);
        width = clientArea.right - clientArea.left;
        height = clientArea.bottom - clientArea.top;
    }
    else
    {
        width = inSpec.RequiredWidth;
        height = inSpec.RequiredHeight;
    }

    // Create an orthogonal projection transformation
    ProjMatrix.makeOrthoProjLH((float)width, (float)height, .1f, 10.f);

    // Create a d3d viewport that simply fills up the render target
    D3DViewport.create();
    D3D11_VIEWPORT& vp = D3DViewport.get();
    vp.Width    = (float)width;
    vp.Height   = (float)height;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
}

void GuiDrawInterface::GuiViewport::resize(HWND hwnd, const Vector2D& newSize)
{
    // resize render target
    D3DRenderTarget->resize((uint32_t)newSize.x, (uint32_t)newSize.y);

    // recreate orthogonal projection transformation
    ProjMatrix.makeOrthoProjLH(newSize.x, newSize.y, 0.1f, 2.f);

    // resize d3d viewport
    D3D11_VIEWPORT& vp = D3DViewport.get();
    vp.Width  = newSize.x;
    vp.Height = newSize.y;
}
