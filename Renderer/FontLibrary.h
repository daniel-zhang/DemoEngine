#pragma once
#include "RendererCommon.h"
#include "TextureAtlas.h"

struct FontDescription
{
    std::string FontName;
    uint32_t FontSize; // Size in pixels
};

struct FontDescHash 
{
    size_t operator()(const FontDescription& key) const
    {
        size_t val = 0;
        MurmurHash3_x86_32((const void*)&key.FontName[0], key.FontName.size(), val, &val);
        MurmurHash3_x86_32((const void*)&key.FontSize, sizeof uint32_t, val, &val);
        return val;
    }
};

struct FontDescEqual
{
    bool operator()(const FontDescription& lhs, const FontDescription& rhs) const
    {
        return  lhs.FontName == rhs.FontName && lhs.FontSize == rhs.FontSize;
    }
};


/*
Describes how to render a character. This is the primary output of the font library.
*/
struct CharacterMetrics
{
    CharacterMetrics(){ memset(this, 0, sizeof CharacterMetrics); }

    // The character to be rendered
    wchar_t Character;

    // Character layout in pixel unit, coordinates: X-right, Y-Up
    uint32_t PixelWidth;
    uint32_t PixelHeight;
    int32_t XOffset;
    int32_t YOffset;
    int32_t XAdvance;
    int32_t GlobalMaxHeight;
    int32_t GlobalDescender;

    // The texture atlas
    //DynamicTexture2D* FontTexture;
    Texture* FontTexture;

    // Normalized UV coordinates that define the character's texture rectangle in texture atlas
    float UStart;
    float VStart;
    float USize;
    float VSize;
};

class FontLibrary
{
public:
    static FontLibrary* getInstance()
    {
        static FontLibrary instance;
        return &instance;
    }
    FontLibrary();
    ~FontLibrary();

public:
    // Interface that GuiDrawInterface talks to
    void initialize(bool enablePreCache = true);
    void destroy();
    void getMetrics(const wchar_t inChar, const FontDescription& inFontDesc, CharacterMetrics& outCharMetrics);
    bool hasKerning(const FontDescription& inFontDesc) const;
    int8_t getKerning(const FontDescription& inFontDesc, const wchar_t first, const wchar_t second) ;

private:
    FontLibrary(const FontLibrary&);
    FontLibrary& operator=(const FontLibrary&);

private:
    struct FontCache
    {
        // Metrics of ASCII characters defined by this font
        std::vector<CharacterMetrics> FontMetrics;

        // Describes how special pair of adjacent characters have their layout adjusted
        std::vector<int8_t> KerningTable;
    };

private:
    void cache(const FontDescription& inFontDesc);

    // List all font files under default font path and cache them all
    void precache();

private:
    std::unique_ptr<class FreetypeInterface> mFreetypeInterface;
    std::unordered_map<FontDescription, FontCache, FontDescHash, FontDescEqual> mFontCacheMap;
    TextureAtlas mTextureAtlas;

    const uint32_t mDpiX;
    const uint32_t mDpiY;
    const uint32_t mCharNum;
    const std::string mDefaultFontFilename;
};
