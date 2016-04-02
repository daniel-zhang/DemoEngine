#include "Core/Util.h"
//#include "Engine/Application.h"

#include "Renderer/TestRenderer.h"
#include "Renderer/TestGuiDraw.h"
#include "Renderer/TestDraw2D.h"
#include "Renderer/Texture.h"

/*********************************************************************************************
Bypass the engine to directly test subsystem functionalities.
*********************************************************************************************/

#include "TestImage.h"

int WINAPI wWinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // tiff to png
    // tiff to jpg
    // tiff to dds
    // dds to png
    // jpg to png
    // png to jpg
    TestImage::convert(L"Adium.png", Image::BMP);
    TestImage::convert(L"Adium.png", Image::JPEG);
    TestImage::convert(L"Adium.png", Image::TGA);
    TestImage::convert(L"Adium.png", Image::DDS);
    TestImage::convert(L"Adium.png.dds", Image::PNG);
    TestImage::convert(L"kitty.gif", Image::JPEG);
    TestImage::convert(L"kitty.gif", Image::PNG);


    // Test textures 
    Renderer::getInstance()->initialize(RendererSettings());

    Texture tex;

    // dynamic 1d
    tex.initializeAsDynamic(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 1, 1);
    tex.destroy();

    // dynamic 2d
    tex.initializeAsDynamic(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 1);
    tex.destroy();

    // dynamic 3d
    tex.initializeAsDynamic(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 30);
    tex.destroy();

    // dsv
    tex.initializeAsDepthStencil(DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R32_FLOAT, 500, 500, 1);
    tex.destroy();

    // dsv array
    tex.initializeAsDepthStencil(DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R32_FLOAT, 500, 500, 5);
    tex.destroy();

    // rtv
    tex.initializeAsRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 1);
    tex.destroy();

    // rtv array
    tex.initializeAsRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 5);
    tex.destroy();

    // immutable 
    std::wstring dir; 
    TestImage::getDir(dir);
    Image img;
    img.createFromFile(dir + L"Adium.png");

    tex.initializeAsImmutableFromImage(img);
    tex.debugSave(dir + L"Adium_save_from_immutable", Image::PNG);
    tex.destroy();

    // Test dynamic texture update/commit
    Texture dynamicTex;
    dynamicTex.initializeAsDynamic(img.getDxgiFormat(), img.getWidth(), img.getHeight(), img.getDepth());
    dynamicTex.updateImageCache() = img;
    dynamicTex.commitImageCache_Tex2D();
    dynamicTex.debugSave(dir + L"Adium_save_from_dynamic", Image::PNG);
    dynamicTex.destroy();

    // TODO: debug save + texture copy

    /*
    tex.initialize(Texture::EUsage::DepthStencilAndShaderResource, 400, 400, 1, DXGI_FORMAT_D24_UNORM_S8_UINT);
    tex.destroy();

    tex.initialize(Texture::EUsage::RenderTargetAndShaderResource, 200, 200, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
    tex.destroy();


    std::wstring dir; 
    TestImage::getDir(dir);
    Image img;

    img.createFromFile(dir + L"Adium.png");

    tex.initializeFromImage(img, Texture::DynamicShaderResource);
    tex.destroy();

    tex.initializeFromImage(img, Texture::ImmutableShaderResource);
    tex.destroy();
    */

    /*
    Window* noBorderWnd = Window::buildBorderlessWindow();
    //Window* defaultWnd = Window::buildDefaultWindow();

    Application::getInstance()->addWindow(noBorderWnd);
    //Application::getInstance()->addWindow(defaultWnd);

    std::vector<HWND> hwnds;
    //hwnds.push_back(defaultWnd->getNativeWindow()->getHwnd());
    hwnds.push_back(noBorderWnd->getNativeWindow()->getHwnd());


    //TEST_ShaderCompiler();
    //TEST_Serialization();
    //TEST_FbxLoader();

    /*
    TestRenderer tRenderer;
    tRenderer.initialize(hwnd);
    TestDraw2D testDraw2D;
    testDraw2D.initialize(hwnd);
   */
    /*

    GuiDrawTest guiDrawTest;
    guiDrawTest.initialize(hwnds);

    TickTimer timer;
    timer.reset();

    Application::getInstance()->run( 
        [&]()->void
        {
            float dt = (float)timer.tick();
            //tRenderer.testDraw(dt);
            guiDrawTest.testDraw(dt);
            //testDraw2D.testDraw();
        }
    );
*/
    CoUninitialize();
    return 0;
}