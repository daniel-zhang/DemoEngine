#include "FontLibrary.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H


/*A simple wrapper to hide FreeType code from public headers*/
class FreetypeInterface
{
public:
    ~FreetypeInterface(){ destroy(); }
    void initialize()
    {
        FT_Error error = FT_Init_FreeType(&mFreetypeLib);
        vcheck(error == 0, L"Failed to initialize Freetype2 library.");
    }

    void destroy()
    {
        FT_Done_FreeType(mFreetypeLib);
    }

    FT_Library& getFT()
    {
        return mFreetypeLib;
    }

protected:
    FT_Library mFreetypeLib;
};

FontLibrary::FontLibrary() :
    mFreetypeInterface(nullptr),
    mDpiX(96), mDpiY(96), mCharNum(256),
    mDefaultFontFilename("segoeui.ttf")
{
}

FontLibrary::~FontLibrary()
{
    destroy();
}

void FontLibrary::initialize(bool enablePreCache)
{
    mFreetypeInterface.reset(new FreetypeInterface());
    mFreetypeInterface->initialize();

    if (enablePreCache)
    {
        precache();
        mTextureAtlas.debugDumpTextures();
    }
}

void FontLibrary::destroy()
{
    mFreetypeInterface.reset(nullptr);
}

void FontLibrary::getMetrics(const wchar_t inChar, const FontDescription& inFontDesc, CharacterMetrics& outCharMetrics)
{
    auto result = mFontCacheMap.find(inFontDesc);
    if (result == mFontCacheMap.end())
    {
        cache(inFontDesc);
        result = mFontCacheMap.find(inFontDesc);
    }
    check(result != mFontCacheMap.end());

    outCharMetrics = result->second.FontMetrics[(uint32_t)inChar];
}

bool FontLibrary::hasKerning(const FontDescription& inFontDesc) const
{
    auto result = mFontCacheMap.find(inFontDesc);
    if (result == mFontCacheMap.end())
    {
        return false;
    }
    return !( result->second.KerningTable.empty() );
}

int8_t FontLibrary::getKerning(const FontDescription& inFontDesc, const wchar_t first, const wchar_t second) 
{
    auto result = mFontCacheMap.find(inFontDesc);
    if (result == mFontCacheMap.end())
    {
        cache(inFontDesc);
        result = mFontCacheMap.find(inFontDesc);
    }
    check(result != mFontCacheMap.end());

    return result->second.KerningTable[first * mCharNum + second];
}

void FontLibrary::cache(const FontDescription& inFontDesc)
{
    // Make a copy of incoming font desc, we might perform a fall back cache on it if its font file is not available
    FontDescription fontDesc = inFontDesc;

    // Check if required font file is available
    std::string defaultFontFileDir = ws2s(FileOp::getDefaultPath<FileOp::FontResource>().toString());
    std::string fontFilePath= defaultFontFileDir + fontDesc.FontName;
    if ( !FileOp::checkFileExists(fontFilePath) )
    {
        // Fall back to default font file
        fontFilePath = defaultFontFileDir + mDefaultFontFilename;
        fontDesc.FontName = mDefaultFontFilename;
    }
    vcheck(FileOp::checkFileExists(fontFilePath), L"Default font file not available.");
    
    // Now start to cache
    auto result = mFontCacheMap.emplace(fontDesc, FontCache());

    // Already cached, skip.
    if (result.second == false)
        return;

    // Not cached yet, let's make a cache for the incoming font
    FontCache& fontCache = result.first->second;

    // Create a FreeType font face from font file
    FT_Face ftFontFace;
    FT_Error error = FT_New_Face(mFreetypeInterface->getFT(), fontFilePath.c_str(), 0, &ftFontFace);
    vcheck(error == 0, L"Failed to extract font face");

    // Set the desired size of the font face
    FT_Set_Char_Size( ftFontFace, fontDesc.FontSize* 64, fontDesc.FontSize* 64, mDpiX, mDpiY);

    // Construct a kerning table 
    if (FT_HAS_KERNING(ftFontFace))
    {
        fontCache.KerningTable.resize(mCharNum * mCharNum, 0);
        FT_Vector kerningVector;
        for (wchar_t firstChar = 0; firstChar < mCharNum; ++firstChar)
        {
            for (wchar_t secondChar = 0; secondChar < mCharNum; ++secondChar)
            {
                FT_UInt firstInd = FT_Get_Char_Index(ftFontFace, firstChar);
                FT_UInt secondInd = FT_Get_Char_Index(ftFontFace, secondChar);

                FT_Get_Kerning(ftFontFace, firstInd, secondInd, FT_KERNING_DEFAULT, &kerningVector);

                // Convert kerning to pixel unit and cache it (we only care about horizontal text layout)
                fontCache.KerningTable[firstChar * mCharNum + secondChar] = (int8_t) kerningVector.x >> 6;
            }
        }
    }

    // Now use FreeType to render ASCII char
    fontCache.FontMetrics.resize(mCharNum);
    for (uint32_t i = 0; i < fontCache.FontMetrics.size(); ++i)
    {
        CharacterMetrics& cm = fontCache.FontMetrics[i];

        cm.Character = (wchar_t)i; // Unicode and ASCII share indices from 0 to 255

        FT_UInt ftCharIndex = FT_Get_Char_Index(ftFontFace, cm.Character);

        // Skip non-printable characters
        if (ftCharIndex == 0)
            continue;

        // Set "FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT" to ensure FreeType's auto hint algorithm is always used
        FT_Load_Glyph(ftFontFace, ftCharIndex, FT_LOAD_NO_BITMAP | FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT);

        // Now render the char glyph 
        FT_GlyphSlot glyphSlot = ftFontFace->glyph;
        FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);

        // Deal with format chars. No bitmap rendered implies a format char (e.g. space, nbsp, or \r)
        // Note, a format char is still considered to be printable 
        if (glyphSlot->bitmap.buffer == NULL)
        {
            // Since We are only interested in horizontal text layout, XAdvance is all we care.
            cm.XAdvance = glyphSlot->advance.x >> 6;
            continue;
        }

        // Upon this point, the char glyph bitmap is good and ready to be added to texture atlas
        // Note, the glyph bitmap is a 1 channel gray scale bitmap
        AtlasRect outRect;
        mTextureAtlas.feedBitmap(glyphSlot->bitmap.buffer, glyphSlot->bitmap.width, glyphSlot->bitmap.rows, outRect);

        uint32_t textureWidth = outRect.AtlasTexture->getWidth();
        uint32_t textureHeight = outRect.AtlasTexture->getHeight();

        cm.FontTexture = outRect.AtlasTexture;
        cm.PixelWidth  = outRect.Width;
        cm.PixelHeight = outRect.Height;

        cm.UStart = (float)outRect.X / (float)textureWidth;
        cm.VStart = (float)outRect.Y / (float)textureHeight;
        cm.USize  = (float)outRect.Width / (float)textureWidth;
        cm.VSize  = (float)outRect.Height / (float)textureHeight;

        cm.XOffset         = glyphSlot->bitmap_left;
        cm.YOffset         = glyphSlot->bitmap_top;
        cm.XAdvance        = glyphSlot->advance.x >> 6;
        cm.GlobalMaxHeight = FT_MulFix(ftFontFace->height, ftFontFace->size->metrics.y_scale) >> 6;
        cm.GlobalDescender = ftFontFace->size->metrics.descender >> 6;
    }

    // Clean up the font face
    FT_Done_Face(ftFontFace);

    // Commit textures of these font cache 
    mTextureAtlas.commit();
}

void FontLibrary::precache()
{
    const static uint32_t fontSizeArray[] = {18, 22, 26};
    const static uint32_t numSizes = sizeof fontSizeArray / sizeof uint32_t;

    // List all font files under default font path
    FileList fileList;
    FileOp::listFiles(FileOp::getDefaultPath<FileOp::FontResource>(), L"ttf", fileList);

    for (auto fontFile : fileList.getList())
    {
        FontDescription fd;
        fd.FontName = ws2s(fontFile.getName());
        for (uint32_t sizeIndex = 0; sizeIndex < numSizes; ++sizeIndex)
        {
            fd.FontSize = fontSizeArray[sizeIndex];
            cache(fd);
        }
    }
}


