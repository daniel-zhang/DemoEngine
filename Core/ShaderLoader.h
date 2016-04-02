#pragma once

#include "Compiler.h"

class ShaderCacheFile
{
public:
    CompilerInput Input;
    CompilerOutput Output;
    uint32_t SrcFileHash;

    static const wchar_t* getExt() { return L".csc"; /* Compiled Shader Cache*/ }

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & Input & Output & SrcFileHash;
    }

    void load(const std::string& filename);

    void save(const std::string& filename);
};

class ShaderLoader
{
public:
    static ShaderLoader* getInstance()
    {
        static ShaderLoader loader;
        return &loader;
    }

public:
    // Either load from disk cache or recompile if disk cache is outdated
    bool load(const ShaderCompilerTarget& target, const std::string& srcFilename, const std::string& entrypoint,
        const ShaderCompileFlags& flags, CompilerOutput& output);

    bool load(CompilerInput& input, CompilerOutput& output);

private:
    ShaderLoader(){}
};