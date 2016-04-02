#include "catch.hpp"

#include "Core/Util.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

TEST_CASE("Texture", "[renderer]") 
{
    Renderer::getInstance()->initialize(RendererSettings());
    Texture tex;

    SECTION("Dynamic Texture 1D") {
        tex.initializeAsDynamic(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 1, 1);
        tex.destroy();
    }

    SECTION("Dynamic Texture 2D") {
        tex.initializeAsDynamic(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 1);
        tex.destroy();
    }

    SECTION("Dynamic Texture 3D") {
        tex.initializeAsDynamic(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 30);
        tex.destroy();
    }

    SECTION("Single DepthStencil") {
        tex.initializeAsDepthStencil(DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R32_FLOAT, 500, 500, 1);
        tex.destroy();
    }

    SECTION("Array of DepthStencil") {
        tex.initializeAsDepthStencil(DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R32_FLOAT, 500, 500, 5);
        tex.destroy();
    }

    SECTION("Single RenderTarget") {
        tex.initializeAsRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 1);
        tex.destroy();
    }

    SECTION("Array of RenderTarget") {
        tex.initializeAsRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 5);
        tex.destroy();
    }

    SECTION("Create immutable Texture2D from Image") {
        std::wstring dir;
        dir = FileOp::getCurrPath().toString();
        dir += L"../FunctionalTest/Image/";

        std::wstring png = dir + L"Adium.png";

        REQUIRE(FileOp::checkFileExists(png) == true);

        Image img;
        img.createFromFile(png);

        tex.initializeAsImmutableFromImage(img);

        std::wstring debug_png = dir + L"Adium_save_from_immutable";
        tex.debugSave(debug_png, Image::PNG);

        tex.destroy();

        REQUIRE(FileOp::checkFileExists(debug_png + L".png") == true);
    }

    SECTION("Dynamic Texture2D update/commit/save"){
        std::wstring dir;
        dir = FileOp::getCurrPath().toString();
        dir += L"../FunctionalTest/Image/";

        std::wstring png = dir + L"Adium.png";

        REQUIRE(FileOp::checkFileExists(png) == true);

        Image img;
        img.createFromFile(png);

        Texture dynamicTex;
        dynamicTex.initializeAsDynamic(img.getDxgiFormat(), img.getWidth(), img.getHeight(), img.getDepth());

        dynamicTex.updateImageCache() = img;
        dynamicTex.commitImageCache_Tex2D();

        std::wstring debug_png = dir + L"Adium_save_from_dynamic";
        dynamicTex.debugSave(debug_png, Image::PNG);

        dynamicTex.destroy();

        REQUIRE(FileOp::checkFileExists(debug_png + L".png") == true);
    }

    Renderer::getInstance()->destroy();
}
