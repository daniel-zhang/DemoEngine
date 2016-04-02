#include "BasePixelShader.h"

const char* BasePixelShader::sourceFilename() const
{
    static const char* filename = "BasePS.hlsl";
    return filename;
}

const char* BasePixelShader::entrypointName() const
{
    static const char* entrypoint = "PSMain";
    return entrypoint;
}


