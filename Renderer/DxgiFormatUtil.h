#pragma once

#include <dxgiformat.h>

// Stolen from DXTex
namespace DxgiFormatUtil
{
bool  isValid(DXGI_FORMAT fmt);

bool  isCompressed(DXGI_FORMAT fmt);

bool  isPacked(DXGI_FORMAT fmt);

bool  isVideo(DXGI_FORMAT fmt);

bool  isPlanar(DXGI_FORMAT fmt);

bool  isPalettized(DXGI_FORMAT fmt);

bool  isDepthStencil(DXGI_FORMAT fmt);

bool  isSRGB(DXGI_FORMAT fmt);

bool  isTypeless(DXGI_FORMAT fmt, bool partialTypeless = true);

bool  hasAlpha(DXGI_FORMAT fmt);

size_t  bitsPerPixel(DXGI_FORMAT fmt);

size_t  bitsPerColor(DXGI_FORMAT fmt);
};
