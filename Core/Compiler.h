#pragma once

#include "Util.h"
#include "ShaderStage.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

class ShaderMacros
{
public:
    ShaderMacros()
    {
        mMacros.reserve(30);
    }

    const std::unordered_map<std::string, std::string>& getMacroDefines() const
    {
        return mMacros;
    }

    void defineString(const std::string& name, const std::string& value)
    {
        mMacros.emplace(name, value);
    }

    void defineInteger(const std::string& name, const uint32_t value)
    {
        mMacros.emplace( name, string_format("%u", value) );
    }

    void defineFloat(const std::string& name, const float value)
    {
        mMacros.emplace( name, string_format("%f", value) );
    }

    void merge(const ShaderMacros& other)
    {
        auto otherMacros = other.getMacroDefines();
        mMacros.insert(otherMacros.begin(), otherMacros.end());
    }

private:
    std::unordered_map<std::string, std::string> mMacros;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & mMacros;
    }
};

class ShaderIncludes
{
public:
    ShaderIncludes()
    {
        mIncludes.reserve(20);
    }

    const std::unordered_map<std::string, std::string>& getIncludes() const
    {
        return mIncludes;
    }

    void addInclude(const std::string& includeFilename, const std::string& content)
    {
        mIncludes.emplace(includeFilename, content);
    }

    void merge(const ShaderIncludes& other)
    {
        mIncludes.insert(other.mIncludes.begin(), other.mIncludes.end());
    }

private:
    std::unordered_map<std::string, std::string> mIncludes;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & mIncludes;
    }
};

class ShaderCompileFlags 
{
public:
    ShaderCompileFlags() : mCompileFlags(0) {}

    void reset()
    {
        mCompileFlags = 0;
    }

    void addFlag(const uint32_t flag)
    {
        mCompileFlags |= flag;
    }

    const uint32_t& getFlags() const
    {
        return mCompileFlags;
    }

    void merge(const ShaderCompileFlags& other)
    {
        mCompileFlags |= other.getFlags();
    }

private:
    uint32_t mCompileFlags;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & mCompileFlags;
    }
};

class ShaderParamAllocation
{
public:
    ShaderParamAllocation():IsBound(false){}

    bool IsBound; // Is this param bound
    uint32_t BufferIndex; // Which shader buffer
    uint32_t ByteOffset; // Offset into this shader buffer
    uint32_t ByteSize; // Size of this param 

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & IsBound & BufferIndex & ByteOffset & ByteSize;
    }
};

class CBufferDesc
{
public:
    CBufferDesc() : BufferIndex(0), BufferSize(0), NumVars(0) {}

    CBufferDesc(uint32_t index, const char* name, uint32_t byteSize, uint32_t numVars)
        : BufferIndex(index), Name(name), BufferSize(byteSize), NumVars(numVars)
    {}

public:
    std::string Name;
    uint32_t BufferIndex;
    uint32_t BufferSize;
    uint32_t NumVars;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & Name & BufferIndex & BufferSize & NumVars;
    }
};

class ShaderParamMap
{
public:
    ShaderParamMap() { }

    ShaderParamMap(const ShaderParamMap& rhs) { *this = rhs; }

    ShaderParamMap& operator=(const ShaderParamMap& rhs)
    {
        mParamMap.clear();
        mParamMap.insert(rhs.mParamMap.begin(), rhs.mParamMap.end());

        mCBufferDescs.clear();
        mCBufferDescs.insert(mCBufferDescs.begin(), rhs.mCBufferDescs.begin(), rhs.mCBufferDescs.end());

        return *this;
    }

    bool checkIfAllParamsAreBound()
    {
        bool result = true;
        for (auto param : mParamMap)
        {
            result = result && param.second.IsBound;
        }
        return result;
    }

    void addCBufferDesc(uint32_t bufferIndex, const char* name, uint32_t byteSize, uint32_t numVars)
    {
        mCBufferDescs.emplace_back(bufferIndex, name, byteSize, numVars);
    }

    const std::vector<CBufferDesc>& getCBufferDescs() const
    {
        return mCBufferDescs;
    }

    void addParam(const std::string& paramName, const ShaderParamAllocation& inParam)
    {
        mParamMap.emplace(paramName, inParam);
    }

    // Output parameter memory layout
    bool findParamByName(const std::string& paramName, ShaderParamAllocation& outParam) const
    {
        auto result = mParamMap.find(paramName);
        if ( result!= mParamMap.end() )
        {
            outParam = result->second;
            return true;
        }
        return false;
    }

    // Output parameter memory layout
    // Internally set the parameter as bound
    bool bindParamByName(const std::string& paramName, ShaderParamAllocation& outParam)
    {
        auto result = mParamMap.find(paramName);
        if ( result!= mParamMap.end() )
        {
            result->second.IsBound = true;
            outParam = result->second;
            return true;
        }
        return false;
    }

    bool isParamExisted(const std::string& paramName) const
    {
        return ( mParamMap.find(paramName) != mParamMap.end() );
    }

    const std::unordered_map<std::string, ShaderParamAllocation>& getMap() const
    {
        return mParamMap;
    }

private:
    std::unordered_map<std::string, ShaderParamAllocation> mParamMap;

    std::vector<CBufferDesc> mCBufferDescs;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & mParamMap & mCBufferDescs;
    }
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ShaderCompilerTarget
{
public:
    ShaderCompilerTarget():mTarget(VertexShader_SM5)
    {
    }

    void setTarget(EShaderStage inTarget)
    { 
        mTarget = inTarget; 
    }

    const char* toShaderProfileString() const
    {
        static const char* str[] = {
            "vs_5_0",
            "hs_5_0",
            "ds_5_0",
            "ps_5_0",
            "gs_5_0",
            "cs_5_0",
        };
        return str[mTarget];
    }

private:
    EShaderStage mTarget;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & mTarget;
    }
};

class CompilerInput
{
public:
    CompilerInput() {}

    ShaderCompilerTarget CompilerTarget;

    ShaderCompileFlags Flags;

    ShaderIncludes Includes;

    ShaderMacros Macros;

    std::string SrcFileName; 

    std::string EntryPointName;

    uint32_t computeHash() const
    {
        uint32_t hash = 0;

        MurmurHash3_x86_32(&CompilerTarget, sizeof(CompilerTarget), 0, &hash);

        MurmurHash3_x86_32(&Flags, sizeof(Flags), hash, &hash);

        const std::unordered_map<std::string, std::string>& includes = Includes.getIncludes();
        for (auto i : includes)
        {
            if (!i.first.empty())
                MurmurHash3_x86_32(&i.first[0], i.first.size() * sizeof(char), hash, &hash);
            
            if (!i.second.empty())
                MurmurHash3_x86_32(&i.second[0], i.second.size() * sizeof(char), hash, &hash);
        }

        MurmurHash3_x86_32(&Flags, sizeof(Flags), hash, &hash);
        const std::unordered_map<std::string, std::string>& macros = Macros.getMacroDefines();
        for (auto m : macros)
        {
            if (!m.first.empty())
                MurmurHash3_x86_32(&m.first[0], m.first.size() * sizeof(char), hash, &hash);
            
            if (!m.second.empty())
                MurmurHash3_x86_32(&m.second[0], m.second.size() * sizeof(char), hash, &hash);
        }

        if (!SrcFileName.empty())
        {
            MurmurHash3_x86_32(&SrcFileName[0], SrcFileName.size() * sizeof(char), hash, &hash);
        }

        if (!EntryPointName.empty())
        {
            MurmurHash3_x86_32(&EntryPointName[0], EntryPointName.size() * sizeof(char), hash, &hash);
        }

        return hash;
    }

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & CompilerTarget & Flags & Includes & Macros & SrcFileName & EntryPointName;
    }
};

class CompilerOutput
{
public:
    struct CompilerError
    {
        std::string ErroredFile;
        std::string ErroredLine;
        std::string ErrorMsg;
        std::string formattedError() const
        {
            return ErroredFile + "(" + ErroredLine + "): " + ErrorMsg;
        }
    };

public:
    CompilerOutput(): 
        ByteCodeHash(0), 
        NumInstructions(0), NumTextureSamplers(0), NumTextures(0),
        Succeeded(false) {}

    ShaderCompilerTarget CompilerTarget;

    ShaderParamMap ParamMap;

    std::vector<uint8_t> ByteCode;

    uint32_t ByteCodeHash;

    uint32_t NumInstructions;

    uint32_t NumTextures;

    uint32_t NumTextureSamplers;

    bool Succeeded;

    std::vector<CompilerError> Errors; // Not for serialization 

    std::string getErrorString()
    {
        std::string errorString;
        for (const CompilerError& e : Errors)
        {
            errorString += e.formattedError() + "\n\n";
        }
        return errorString;
    }

    void generateHash()
    {
        // void MurmurHash3_x86_32(const void * key, int len, uint32_t seed, void * out);
        MurmurHash3_x86_32(&ByteCode[0], ByteCode.size() * sizeof(uint8_t), 0, &ByteCodeHash);
    }

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & CompilerTarget & ParamMap & ByteCode & ByteCodeHash 
            & NumInstructions & NumTextures & NumTextureSamplers & Succeeded;
    }
};


/************************************************************************/
/*                                                                      */
/************************************************************************/
bool compileShader(const CompilerInput& input, CompilerOutput& output, const ShaderMacros& addtionalMacros,
    const std::string& srcContent);

bool compileShader(const CompilerInput& input, CompilerOutput& output, const std::string& srcContent);

void testShaderCompiler2();