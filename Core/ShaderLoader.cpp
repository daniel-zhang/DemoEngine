#include "ShaderLoader.h"
#include <d3dcompiler.h>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

void ShaderCacheFile::load(const std::string& filename)
{
    std::ifstream ifs(filename, std::ifstream::binary);
    boost::archive::binary_iarchive ia(ifs);
    ia >> *this;
}

void ShaderCacheFile::save(const std::string& filename)
{
    std::ofstream ofs(filename, std::ifstream::binary);
    boost::archive::binary_oarchive oa(ofs);
    oa << *this;
}

// Note -- use case not covered: 
// User wants to load the same shader with different input settings(e.g. compiler flags/includes/macros)
bool ShaderLoader::load(const ShaderCompilerTarget& target, const std::string& srcFilename, const std::string& entrypoint, const ShaderCompileFlags& flags, CompilerOutput& output)
{
    std::string path = ws2s(FileOp::getDefaultPath<FileOp::ShaderResource>().toString());
    std::string fullPathName = path + "/" + srcFilename;

    // Check source file exists
    if (FileOp::checkFileExists(fullPathName) == false)
    {
        return false;
    }

    // Load shader source code
    std::string srcFileContent;
    FileOp::readFile(fullPathName, srcFileContent);

    // compute hash for source code
    uint32_t srcHash;
    MurmurHash3_x86_32(&srcFileContent[0], srcFileContent.length(), 0, &srcHash);

    // Construct cache file name
    std::string nakedName, ext;
    FileOp::parseFilename(srcFilename, nakedName, ext);

#if defined(_DEBUG) || defined(DEBUG)
    std::string cacheFilename = path + "/" + nakedName + "_debug" + ws2s(ShaderCacheFile::getExt());
#else
    std::string cacheFilename = path + "/" + nakedName + ws2s(ShaderCacheFile::getExt());
#endif

    // Check if cache file exists
    ShaderCacheFile cacheFile;
    if (FileOp::checkFileExists(cacheFilename))
    {
        cacheFile.load(cacheFilename);

        // Compare cached hash with newest hash
        if (cacheFile.SrcFileHash == srcHash)
        {
            output = cacheFile.Output;
            return true;
        }
    }

    // Cache file not exist or outdated: recompile 
    CompilerInput input;
    input.CompilerTarget = target;
    input.SrcFileName    = fullPathName;
    input.EntryPointName = entrypoint;
    input.Flags          = flags;
#if defined(_DEBUG) || defined(DEBUG)
    input.Flags.addFlag(D3DCOMPILE_DEBUG);
    input.Flags.addFlag(D3DCOMPILE_SKIP_OPTIMIZATION);
#endif

    compileShader(input, output, ShaderMacros(), srcFileContent);

    // Now cache the result for next load
    cacheFile.Input = input;
    cacheFile.Output = output;
    cacheFile.SrcFileHash = srcHash;
    cacheFile.save(cacheFilename);

    return true;
}

bool ShaderLoader::load(CompilerInput& input, CompilerOutput& output)
{
    std::wstring dir = FileOp::getDefaultPath<FileOp::ShaderResource>().toString();

    // Construct source file full name
    std::wstring srcFilename = s2ws(input.SrcFileName);
    std::wstring srcFullname = dir + L"/" + srcFilename;
    input.SrcFileName = ws2s(srcFullname);

    // Construct cache file name
    std::wstring cacheFullname;
    {
        std::wstring nakedName, ext;
        FileOp::parseFilename(srcFilename, nakedName, ext);

#if defined(_DEBUG) || defined(DEBUG)
        cacheFullname = dir + L"/" + nakedName + L"_debug" + ShaderCacheFile::getExt();
#else
        cacheFullname = dir + L"/" + nakedName + ShaderCacheFile::getExt();
#endif
    }

    bool srcFileExists = FileOp::checkFileExists(srcFullname);
    bool cacheFileExists = FileOp::checkFileExists(cacheFullname);

    if (!srcFileExists && !cacheFileExists)
    {
        show_error(wstring_format(L"Shader file %s not found.", srcFullname.c_str()));
        return false;
    }
    else if (!srcFileExists && cacheFileExists) // Load from cache
    {
        ShaderCacheFile cacheFile;
        cacheFile.load(ws2s(cacheFullname));

        if (input.computeHash() != cacheFile.Input.computeHash())
        {
            show_error(wstring_format(L"App tried to use a miss matched shader cache: %s .", cacheFullname.c_str()));
            return false;
        }
        output = cacheFile.Output;
        return true;
    }
    else if (srcFileExists && !cacheFileExists) // Compile from source
    {
        // Load shader source code
        std::string sourceCode;
        FileOp::readFile(ws2s(srcFullname), sourceCode);

        if ( !compileShader(input, output, sourceCode) )
        {
            std::string errStr = output.getErrorString();
            show_error(wstring_format(L"%s", s2ws(errStr).c_str() ));
            return false;
        }

        // Now cache the result for next load
        uint32_t srcHash;
        MurmurHash3_x86_32(&sourceCode[0], sourceCode.length(), 0, &srcHash);

        ShaderCacheFile cacheFile;
        cacheFile.Input       = input;
        cacheFile.Output      = output;
        cacheFile.SrcFileHash = srcHash;

        cacheFile.save(ws2s(cacheFullname));

        return true;
    }
    else // Check source code hash and input setting hash, then load from cache or compile from source
    {
        std::string sourceCode;
        FileOp::readFile(ws2s(srcFullname), sourceCode);

        uint32_t srcHash;
        MurmurHash3_x86_32(&sourceCode[0], sourceCode.length(), 0, &srcHash);

        ShaderCacheFile cacheFile;
        cacheFile.load(ws2s(cacheFullname));

        if (input.computeHash() == cacheFile.Input.computeHash() && cacheFile.SrcFileHash == srcHash)
        {
            output = cacheFile.Output;
        }
        else
        {
            if (!compileShader(input, output, sourceCode))
            {
                std::string errStr = output.getErrorString();
                show_error(wstring_format(L"%s", s2ws(errStr).c_str()));

                return false;
            }

            // Now cache the result for next load
            cacheFile.Input       = input;
            cacheFile.Output      = output;
            cacheFile.SrcFileHash = srcHash;
            cacheFile.save(ws2s(cacheFullname));
        }
        return true;
    }
}
