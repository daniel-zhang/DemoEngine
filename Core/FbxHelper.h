#pragma once

#include "Math.h"
#include "Util.h"
#include <fbxsdk.h>

namespace FbxHelper
{
    class Bone
    {
    public:
        Bone():mFbxNode(nullptr){}

        Bone(FbxNode* node) { parse(node); }

        ~Bone() { clear(); }

        void parse(FbxNode* inNode);

        void clear();

    public:
        const std::string& getName() { return mName; }

        FbxNode* getFbxNode() const { return mFbxNode; }

        void addChild(Bone* bone) { mChildren.push_back(bone); }

        Bone* getChild(uint32_t index) const;

        void setParent(Bone* bone) { mParent = bone; }

        Bone* getParent() const { return mParent; }

        // Get number of direct children bones
        uint32_t getChildNum() const{ return mChildren.size(); }

        // Iterate the bone hierarchy to figure out parent number;
        uint32_t getParentNum() const;

    protected:
        std::string mName;
        Bone* mParent;
        std::vector<Bone*> mChildren;
        FbxNode* mFbxNode;
    };

    class Skeleton
    {
    public:
        Skeleton() :mRootBone(nullptr){}

        Skeleton(Bone* root) { setRootBone(root); }

        ~Skeleton() { clear(); }

        void setRootBone(Bone* root);

        void clear();

    public:
        Bone* getRootBone() const { return mRootBone; }

        uint32_t getTotalBoneNum() const;

        void toString(std::string& out) const;

    protected:
        Bone* mRootBone;
    };

    class Texture
    {
    public:
        Texture():mFbxTexture(nullptr){}

        Texture(const std::string& channelName, FbxTexture* inFbxTexture) { parse(channelName, inFbxTexture); }

        ~Texture() { clear(); }

        void parse(const std::string& channelName, FbxTexture* inFbxTexture);

        void clear() { mFbxTexture = nullptr; }

    protected:
        FbxTexture* mFbxTexture;

        std::string mChannelName; // Which material 'channel' this texture is linked to, e.g. diffuse, specular, normal, etc.
        std::string mTextureTypeString; 
        std::string mName;
        std::string mAbsolutePathName;
        std::string mRelativePathName;

        // TODO 
        Vector2D Translation;
        Vector2D Scale;
        Vector3D Rotation;
    };

    class Material
    {
    public:
        Material() :mFbxSurfaceMaterial(nullptr) {}

        Material(FbxSurfaceMaterial* fbxMaterial) { parese(fbxMaterial); }

        ~Material() { clear(); }

        void parese(FbxSurfaceMaterial* inFbxMaterial);

        void clear();

    protected:
        FbxSurfaceMaterial* mFbxSurfaceMaterial;

        std::string mShadingModel;
        std::string mShadingModel_Validate;
        std::vector<Texture*> mTextures;
        std::vector<std::string> mChannelNames;
        std::vector<std::string> mTextureNames;
        std::vector<std::string> mUVNames;
    };

    // FBX stores data in separate streams, of which the indices can be combined as a signature to identify a unique vertex
    class VertexSignature
    {
    public:
        VertexSignature() : PositionTrait(0), NormalTrait(0), UVTrait(0), TangentTrait(0) {}

        int32_t PositionTrait;
        int32_t NormalTrait;
        int32_t UVTrait;
        int32_t TangentTrait;
    };

    struct SignatureEqual
    {
        bool operator()(const VertexSignature& lhs, const VertexSignature& rhs) const
        {
            return
                lhs.PositionTrait == rhs.PositionTrait &&
                lhs.NormalTrait   == rhs.NormalTrait &&
                lhs.UVTrait       == rhs.UVTrait &&
                lhs.TangentTrait  == rhs.TangentTrait;
        }
    };

    struct SignatureHash
    {
        size_t operator()(const VertexSignature& signature) const
        {
            size_t val = 0;
            MurmurHash3_x86_32(&signature, sizeof VertexSignature, 0, &val);
            return val;
        }
    };

    class Vertex
    {
    public:
        Vertex(){}

    public:
        const Vector4D& getPosition() const { return Position; }

        Vector4D& getPosition() { return Position; }

        const Vector4D& getNormal() const { return Normal; }

        Vector4D& getNormal() { return Normal; }

        const Vector2D& getUV() const { return UV; }

        Vector2D& getUV() { return UV; }

    public:
        void readPosition(FbxMesh* inFbxMesh, int cpIndex, VertexSignature& outSig);

        void readUV(FbxMesh* inFbxMesh, int cpIndex, int uvIndex, VertexSignature& outSig);

        void readNormal(FbxMesh* inFbxMesh, int cpIndex, int pvIndex, VertexSignature& outSig);

        // TODO read tangent, read binormal blah

    protected:
        void setPosition(const FbxVector4& inPos);

        void setNormal(const FbxVector4& inNormal);

        void setUV(const FbxVector2& inUV);

    protected:
        Vector4D Position;
        Vector4D Normal;
        Vector2D UV;
    };

    // Covered following cases
    // 1. divide quads into triangles
    // 2. reuse vertex when duplicated (i.e. adjacent polygons share the same vertex)
    // 3. separate vertex (e.g. a 'control point' on an edge might have more than 1 normals/tangents/etc)
    // 4. generate material group according to vertex-material mapping
    // Not covered:
    // 1. support multiple UV layers
    class VertexStream
    {
    public:
        VertexStream() : 
            mCheckDuplication(true), mReverseWindingOrder(false),
            mNumQuadsDivided(0), mNumVertexReuse(0)
        {}

    public:
        const std::vector<Vertex>& getVertexData() const { return mVertexBuffer; }

        // Merge material groups into a single index buffer, useful when they don't want material groups
        void generateIndexData(std::vector<uint32_t>& outIndices) const
        {
            // Determine the size 
            uint32_t numIndex = 0;
            for (auto materialGroup : mMaterialGroups)
            {
                numIndex += materialGroup.size() * 3;
            }

            outIndices.clear();
            outIndices.reserve(numIndex);

            for (auto materialGroup : mMaterialGroups)
            {
                for (auto triangleIndices : materialGroup)
                {
                    for (uint32_t i = 0; i < 3; ++i)
                    {
                        outIndices.push_back(triangleIndices.Index[i]);
                    }
                }
            }
        }

    public:
        void enableDuplicationCheck(bool flag){ mCheckDuplication = flag; }

        void enableWindingOrderReverseWhenImporting(bool flag){ mReverseWindingOrder = flag; }

        // TODO
        //
        // The idea is to pick up a random triangle, calculate a normal(using current winding order)from its center, and check 
        // how many times the normal intersects with other triangles of the geometry.
        //
        // If intersection times are even, the winding order is good. Otherwise reverse it.
        void correctWindingOrder() { } 

        void reverseWindingOrder();

        void negateZ();

        // Returns memory usage in bytes, optionally constructs a readable string if passed in
        uint32_t calcMemoryUsage(std::string* outString = nullptr) const;

        // numMaterial is mandatory, it must be set beforehand to construct the material reference array,
        // which then allows direct access into material groups by material index
        void preAllocate(uint32_t numMaterial, uint32_t numVertex)
        { 
            mMaterialGroups.resize(numMaterial);
            mVertexBuffer.reserve(numVertex); 
        }

        void addPolygon(Vertex* inPolygon, VertexSignature* inSignaures, uint32_t polySize, uint32_t materialIndex);

    protected:
        uint32_t addVertex(const Vertex& inVertex, const VertexSignature& inSignature);

    protected:
        bool mReverseWindingOrder;
        bool mCheckDuplication;
        std::unordered_map<VertexSignature, uint32_t, SignatureHash, SignatureEqual> mSignatureMap;

        // Vertex
        std::vector<Vertex> mVertexBuffer;

        // Index
        struct IndexedTriangle
        {
            uint32_t Index[3];
        };
        typedef std::vector<IndexedTriangle> TMaterialGroup;
        typedef std::vector<TMaterialGroup> TMaterialGroups; // Indexed by mesh materials
        TMaterialGroups mMaterialGroups;

        // Stats
        uint32_t mNumQuadsDivided;
        uint32_t mNumVertexReuse;
    };

    class MeshStats
    {
    public:
        MeshStats() { clear(); }

        // Overall geometry stats
        int NumTotalPolygons;
        int NumTotalVertices; 
        int NumTotalControlPoints; // FBX jargon for 'position stream'

        // Book-keeping counters for debug
        int Triangle_Counter;
        int Quad_Counter;
        int Position_Counter;
        int Normal_Counter;
        int UV_Counter;

        // Verbose FBX info
        struct ElementInfo
        {
            std::string Name;
            FbxLayerElement::EMappingMode MappingMode;
            FbxLayerElement::EReferenceMode ReferenceMode;
        };
        typedef std::vector<ElementInfo> TLayerInfo;

        TLayerInfo MaterialLayerInfo;
        TLayerInfo NormalLayerInfo;
        TLayerInfo BinormalLayerInfo;
        TLayerInfo TangentLayerInfo;
        TLayerInfo UVLayerInfo;

        void collect(FbxMesh* inFbxMesh);

        void clear(){ memset(this, 0, sizeof MeshStats); }

    protected:
        template<typename Lambda1, typename Lambda2>
        void getLayerInfo(TLayerInfo& out, Lambda1& delegate_GetNum, Lambda2& delegate_GetElement)
        {
            int elementNum = delegate_GetNum();
            for (int i = 0; i < elementNum; ++i)
            {
                FbxLayerElement* element  = delegate_GetElement(i);

                ElementInfo elementInfo;
                elementInfo.Name          = element->GetName();
                elementInfo.MappingMode   = element->GetMappingMode();
                elementInfo.ReferenceMode = element->GetReferenceMode();

                out.push_back(elementInfo);
            }
        }
    };

    class Mesh
    {
    public:
        Mesh() : mFbxMesh(nullptr), mFbxNode(nullptr) { }

        Mesh(FbxNode* inNode) :mFbxMesh(nullptr), mFbxNode(nullptr) { parse(inNode); }

        ~Mesh() { clear(); }

        void parse(FbxNode* inNode);

        void clear();

    public:
        const VertexStream& getVertexStream() const{ return mVertexStream; }

        VertexStream& getVertexStream() { return mVertexStream; }

        uint32_t getMemoryFootprint(std::string* outString = nullptr) const;

        uint32_t getFBXMemoryFootprint(std::string* outString = nullptr) const;

    protected:
        void extractMaterials();

        void extractMeshGeometry();

    protected:
        MeshStats mMeshStats;
        FbxNode* mFbxNode;
        FbxMesh* mFbxMesh;
        std::string mNodeName, mMeshName;
        std::vector<Material*> mMaterials;
        VertexStream mVertexStream;
    };

    class Scene
    {
    public:
        Scene():mFbxScene(nullptr){}

        Scene(FbxScene* inScene):mFbxScene(nullptr){ parse(inScene); }

        ~Scene(){ clear(); }

        void parse(FbxScene* inScene);

        void clear();

    public:
        uint32_t getMeshNum(){ return mMeshes.size(); }

        const Mesh& getMesh(uint32_t meshIndex) const { return *mMeshes[meshIndex]; }

        Mesh& getMesh(uint32_t meshIndex) { return *mMeshes[meshIndex]; }

        uint32_t getSkeletonNum(){ return mSkeletons.size(); }

    protected:
        void extractSkeletons();

        void extractMeshes();

    protected:
        FbxScene* mFbxScene;
        std::vector<Skeleton*> mSkeletons;
        std::vector<Mesh*> mMeshes;
    };

    // Helper functions
    template<typename Lambda>
    void traverseFbx(FbxNode* inNode, Lambda& delegate)
    {
        delegate(inNode);
        for (int i = 0; i < inNode->GetChildCount(); ++i)
        {
            traverseFbx(inNode->GetChild(i), delegate);
        }
    }

    template<typename Lambda>
    void traverseSkeleton(class Bone* bone, Lambda& delegate)
    {
        if (!bone) return;

        delegate(bone);

        uint32_t childNum = bone->getChildNum();
        for (uint32_t i = 0; i < childNum; ++i)
        {
            traverseSkeleton(bone->getChild(i), delegate);
        }
    }

    template<FbxNodeAttribute::EType EFbxAttrType>
    bool hasAttr(FbxNode* node)
    {
        if (!node) return false;

        uint32_t attrNum = node->GetNodeAttributeCount();
        for (uint32_t index = 0; index < attrNum; ++index)
        {
            FbxNodeAttribute* attr = node->GetNodeAttributeByIndex(index);
            if (attr && attr->GetAttributeType() == EFbxAttrType)
                return true;
        }

        return false;
    }


    /*
    void extractSkeletons(FbxScene* fbxScene, std::vector<Skeleton*>& outSkeletons);

    // TODO: add tweak options to allow for try & error
    // e.g. reverse winding order, flip Z for vertex/normal, transform UV...
    // TODO: detect fbx scene coordinate, is this possible?
    void extractMeshes(FbxScene* fbxScene, std::vector<Mesh*>& outMeshes);
    */
}
