#include "FbxHelper.h"

//using namespace FbxHelper;
namespace FbxHelper
{
    void searchSkeletonRoots(FbxNode* node, std::vector<FbxNode*>& outSkeletonRoots)
    {
        std::vector<FbxNode*> nonSkeletonChildren;

        uint32_t childNodeNum = node->GetChildCount();
        for (uint32_t i = 0; i < childNodeNum; ++i)
        {
            FbxNode* childNode = node->GetChild(i);

            if (hasAttr<FbxNodeAttribute::eSkeleton>(childNode))
            {
                outSkeletonRoots.push_back(childNode);
            }
            else
            {
                nonSkeletonChildren.push_back(childNode);
            }
        }

        for (auto node : nonSkeletonChildren)
        {
            searchSkeletonRoots(node, outSkeletonRoots);
        }
    }

    // Build a directed, no-cycle graph of bones from the given root bone 
    void buildSkeleton(Bone* inBone)
    {
        FbxNode* inNode = inBone->getFbxNode();
        check(inNode);

        uint32_t childNodeNum = static_cast<uint32_t>(inNode->GetChildCount());
        for (uint32_t childIndex = 0; childIndex < childNodeNum; ++childIndex)
        {
            FbxNode* childNode = inNode->GetChild(childIndex);
            if (hasAttr<FbxNodeAttribute::eSkeleton>(childNode))
            {
                Bone* childBone = new Bone(childNode);

                // Link parent and children
                inBone->addChild(childBone);
                childBone->setParent(inBone);

                buildSkeleton(childBone);
            }
        }
    }

    /*
    void extractSkeletons(FbxScene* fbxScene, std::vector<Skeleton*>& outSkeletons)
    {
        check(fbxScene);
        FbxNode* root = fbxScene->GetRootNode();
        check(root);

        outSkeletons.clear();

        // FbxScene might contain more than one skeleton
        std::vector<FbxNode*> fbxSkeletonRoots;
        if (hasAttr<FbxNodeAttribute::eSkeleton>(root))
        {
            fbxSkeletonRoots.push_back(root);
        }
        else
        {
            searchSkeletonRoots(root, fbxSkeletonRoots);
        }

        // Now build each skeleton from respective skeleton root
        for (auto root : fbxSkeletonRoots)
        {
            Bone* rootBone = new Bone(root);
            buildSkeleton(rootBone);

            Skeleton* skeleton = new Skeleton(rootBone);

            // Debug stats
            uint32_t numBones = skeleton->getTotalBoneNum();
            std::string skeletonString;
            skeleton->toString(skeletonString);

            outSkeletons.push_back(skeleton);
        }
    }

    void FbxHelper::extractMeshes(FbxScene* fbxScene, std::vector<Mesh*>& outMeshes)
    {
        check(fbxScene);
        FbxNode* root = fbxScene->GetRootNode();
        check(root);

        outMeshes.clear();

        traverseFbx(root, [&](FbxNode* inNode)->void{
            if (hasAttr<FbxNodeAttribute::eMesh>(inNode))
            {
                outMeshes.push_back(new Mesh(inNode));
            }
        });
    }
    */

    void Bone::parse(FbxNode* inNode)
    {
        check(inNode);

        mFbxNode = inNode;
        mName = mFbxNode->GetName();
    }

    void Bone::clear()
    {
        for (auto child : mChildren)
        {
            if (child)
            {
                delete child;
                child = nullptr;
            }
        }
        mChildren.clear();
    }

    Bone* Bone::getChild(uint32_t index) const
    {
        if (mChildren.empty() == false && index < mChildren.size())
            return mChildren[index];
        return nullptr;
    }

    uint32_t Bone::getParentNum() const
    {
        uint32_t numParent = 0;
        Bone* parent = this->getParent();
        while (parent)
        {
            ++numParent;
            parent = parent->getParent();
        }
        return numParent;
    }


    void Skeleton::setRootBone(Bone* root)
    {
        check(root);
        clear();
        mRootBone = root;
    }

    void Skeleton::clear()
    {
        if (mRootBone)
        {
            delete mRootBone;
            mRootBone = nullptr;
        }
    }

    uint32_t Skeleton::getTotalBoneNum() const
    {
        if (!mRootBone) return 0;

        uint32_t counter = 0;
        traverseSkeleton(mRootBone, [&counter](Bone*)->void{ ++counter; });

        return counter;
    }

    void Skeleton::toString(std::string& out) const
    {
        if (!mRootBone) return;

        uint32_t indentLevel = 0;

        auto calcIndent = [](Bone* bone)->std::string{
            uint32_t parentNum = bone->getParentNum();
            std::string indent;
            for (uint32_t i = 0; i < parentNum; ++i)
                indent += "--";
            return indent;
        };

        traverseSkeleton(mRootBone, [&](Bone* bone)->void{
            out += calcIndent(bone) + bone->getName() + "\n";
            ++indentLevel;
        });
    }


    void Texture::parse(const std::string& channelName, FbxTexture* inFbxTexture)
    {
        check(inFbxTexture);

        clear();

        mChannelName = channelName;
        mFbxTexture = inFbxTexture;
        mName = mFbxTexture->GetName();

        if (FbxFileTexture* fft = FbxCast<FbxFileTexture>(mFbxTexture))
        {
            mTextureTypeString = "FbxFileTexture";
            mAbsolutePathName = fft->GetFileName();
            mRelativePathName = fft->GetRelativeFileName();
        }
        if (FbxLayeredTexture* flt = FbxCast<FbxLayeredTexture>(mFbxTexture))
        {
            mTextureTypeString = "FbxLayeredTexture";

            vcheck(false, L"TODO: Process FbxLayeredTexture");
        }
        if (FbxProceduralTexture* fpt = FbxCast<FbxProceduralTexture>(mFbxTexture))
        {
            mTextureTypeString = "FbxProceduralTexture";

            vcheck(false, L"TODO: Process FbxProceduralTexture");
        }
    }


    void Material::parese(FbxSurfaceMaterial* inFbxMaterial)
    {
        check(inFbxMaterial);

        clear();

        mFbxSurfaceMaterial = inFbxMaterial;

        // Shading model string
        mShadingModel = mFbxSurfaceMaterial->ShadingModel.Get().Buffer();

        // Find out actual material type
        if (mFbxSurfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
        {
            mShadingModel_Validate = "Phong";

            FbxSurfacePhong* phongMaterial = FbxCast<FbxSurfacePhong>(mFbxSurfaceMaterial);
            check(phongMaterial);

            FbxDouble3 ambient = phongMaterial->Ambient;
            FbxDouble3 diffuse = phongMaterial->Diffuse;
            FbxDouble diffuseFactor = phongMaterial->DiffuseFactor;
            FbxDouble3 specular = phongMaterial->Specular;
            FbxDouble3 emissive = phongMaterial->Emissive;
            FbxDouble opacity = phongMaterial->TransparencyFactor;
            FbxDouble shininess = phongMaterial->Shininess;
            FbxDouble reflectivty = phongMaterial->ReflectionFactor;
        }
        else if (mFbxSurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
        {
            mShadingModel_Validate = "Lambert";
            FbxSurfacePhong* phongMaterial = FbxCast<FbxSurfacePhong>(mFbxSurfaceMaterial);
            check(phongMaterial);

            vcheck(false, L"TODO: Process Lambert Material");
        }
        else
        {
            mShadingModel_Validate = "Unknown";
            vcheck(false, L"Invalid FbxSurfaceMaterial type");
        }

        // Iterate texture channels
        for (int index = 0; index < FbxLayerElement::sTypeTextureCount; ++index)
        {
            std::string channelName = FbxLayerElement::sTextureChannelNames[index];
            std::string textureName = FbxLayerElement::sTextureNames[index];
            std::string uvName = FbxLayerElement::sTextureUVNames[index];

            mChannelNames.push_back(channelName); // Record all supported channels
            mTextureNames.push_back(textureName);
            mUVNames.push_back(uvName);

            FbxProperty fbxProperty = mFbxSurfaceMaterial->FindProperty(channelName.c_str());

            if (fbxProperty.IsValid())
            {
                // Find out which channel has a texture
                FbxTexture* fbxTexture = fbxProperty.GetSrcObject<FbxTexture>(0);
                if (fbxTexture)
                {
                    Texture* texture = new Texture(channelName, fbxTexture);
                    mTextures.push_back(texture);
                }
            }
        }
    }

    void Material::clear()
    {
        mFbxSurfaceMaterial = nullptr;
        for (auto texture : mTextures)
        {
            if (texture)
            {
                delete texture;
                texture = nullptr;
            }
        }
    }
    void Vertex::setPosition(const FbxVector4& inPos)
    {
        Position.x = (float)inPos[0];
        Position.y = (float)inPos[1];
        Position.z = (float)inPos[2];
        Position.w = (float)inPos[3];
    }

    void Vertex::setNormal(const FbxVector4& inNormal)
    {
        Normal.x = (float)inNormal[0];
        Normal.y = (float)inNormal[1];
        Normal.z = (float)inNormal[2];
        Normal.w = (float)inNormal[3];
    }

    void Vertex::setUV(const FbxVector2& inUV)
    {
        UV.x = (float)inUV[0];
        UV.y = (float)inUV[1];
    }

    void Vertex::readPosition(FbxMesh* inFbxMesh, int cpIndex, VertexSignature& outSig)
    {
        FbxVector4 position = inFbxMesh->GetControlPointAt(cpIndex);
        setPosition(position);

        outSig.PositionTrait = cpIndex;
    }

    void Vertex::readUV(FbxMesh* inFbxMesh, int cpIndex, int uvIndex, VertexSignature& outSig)
    {
        FbxGeometryElementUV* element = inFbxMesh->GetElementUV(0);
        if (!element) return;

        FbxLayerElement::EMappingMode mm = element->GetMappingMode();
        FbxLayerElement::EReferenceMode rm = element->GetReferenceMode();

        FbxVector2 uv;
        int targetIndex = 0;

        switch (mm)
        {
        case FbxLayerElement::eByControlPoint:
            targetIndex = cpIndex;
            break;

        case FbxLayerElement::eByPolygonVertex:
            targetIndex = uvIndex;
            break;
        default:
            vcheck(false, L"Invalid mapping mode while reading from FBX");
            break;
        }

        // ReferenceMode defines how to use that index -- but UV's reference mode makes no difference.
        switch (rm)
        {
        case FbxLayerElement::eIndexToDirect:
        case FbxLayerElement::eDirect:
            uv = element->GetDirectArray().GetAt(targetIndex);
            break;
        default:
            vcheck(false, L"Invalid reference mode while reading from FBX");
            break;
        }

        setUV(uv);
        outSig.UVTrait = targetIndex;
    }

    void Vertex::readNormal(FbxMesh* inFbxMesh, int cpIndex, int pvIndex, VertexSignature& outSig)
    {
        FbxGeometryElementNormal* element = inFbxMesh->GetElementNormal(0);
        if (!element) return;

        FbxLayerElement::EMappingMode mm = element->GetMappingMode();
        FbxLayerElement::EReferenceMode rm = element->GetReferenceMode();

        FbxVector4 normal;
        int targetIndex = 0;
        switch (mm)
        {
        case FbxLayerElement::eByControlPoint:
            targetIndex = cpIndex;
            break;

        case FbxLayerElement::eByPolygonVertex:
            targetIndex = pvIndex;
            break;
        default:
            vcheck(false, L"Invalid mapping mode while reading from FBX");
            break;
        }

        switch (rm)
        {
        case FbxLayerElement::eDirect:
            normal = element->GetDirectArray().GetAt(targetIndex);
            break;
        case FbxLayerElement::eIndexToDirect:
            targetIndex = element->GetIndexArray().GetAt(targetIndex);
            normal = element->GetDirectArray().GetAt(targetIndex);
            break;
        default:
            vcheck(false, L"Invalid reference mode while reading from FBX");
            break;
        }

        setNormal(normal);
        outSig.NormalTrait = targetIndex;
    }


    void VertexStream::reverseWindingOrder()
    {
        for (auto materialGroup : mMaterialGroups)
        {
            for (auto triangleIndices : materialGroup)
            {
                // Swap second and third
                uint32_t tmp = triangleIndices.Index[1];
                triangleIndices.Index[1] = triangleIndices.Index[2];
                triangleIndices.Index[2] = tmp;
            }
        }
    }

    void VertexStream::negateZ()
    {
        for (auto vertex : mVertexBuffer)
        {
            vertex.getPosition().z = vertex.getPosition().z * (-1);
        }
    }

    uint32_t VertexStream::calcMemoryUsage(std::string* outString /*= nullptr*/) const
    {
        uint32_t memUsage = mVertexBuffer.size() * sizeof Vertex;
        for (auto materialGroup : mMaterialGroups)
        {
            memUsage += materialGroup.size() * sizeof IndexedTriangle;
        }

        if (outString)
        {
            (*outString) += string_format("%.0f KB", (float)memUsage / 1024);
            (*outString) += string_format(" (%.2f MB)", (float)memUsage / 1024 / 1024);
        }
        return memUsage;
    }

    void VertexStream::addPolygon(Vertex* inPolygon, VertexSignature* inSignaures, uint32_t polySize, uint32_t materialIndex)
    {
        check(polySize == 3 || polySize == 4);

        uint32_t indices[4] = { 0 };
        for (size_t i = 0; i < polySize; ++i)
        {
            indices[i] = addVertex(inPolygon[i], inSignaures[i]);
        }

        if (!mReverseWindingOrder)
        {
            IndexedTriangle triangle_1;
            triangle_1.Index[0] = indices[0];
            triangle_1.Index[1] = indices[1];
            triangle_1.Index[2] = indices[2];
            mMaterialGroups[materialIndex].push_back(triangle_1);
            if (polySize == 4)
            {
                IndexedTriangle triangle_2;
                triangle_2.Index[0] = indices[0];
                triangle_2.Index[1] = indices[2];
                triangle_2.Index[2] = indices[3];
                mMaterialGroups[materialIndex].push_back(triangle_2);
                mNumQuadsDivided++;
            }
        }
        else
        {
            IndexedTriangle triangle_1;
            triangle_1.Index[0] = indices[0];
            triangle_1.Index[1] = indices[2];
            triangle_1.Index[2] = indices[1];
            mMaterialGroups[materialIndex].push_back(triangle_1);
            if (polySize == 4)
            {
                IndexedTriangle triangle_2;
                triangle_2.Index[0] = indices[0];
                triangle_2.Index[1] = indices[3];
                triangle_2.Index[2] = indices[2];
                mMaterialGroups[materialIndex].push_back(triangle_2);
                mNumQuadsDivided++;
            }
        }
    }

    uint32_t VertexStream::addVertex(const Vertex& inVertex, const VertexSignature& inSignature)
    {
        if (mCheckDuplication)
        {
            uint32_t nextIndex = mVertexBuffer.size();
            auto result = mSignatureMap.emplace(inSignature, nextIndex);
            // For duplicated signature, just reuse the stored index 
            if (result.second == false)
            {
                mNumVertexReuse++;
                return result.first->second;
            }
        }
        mVertexBuffer.push_back(inVertex);
        return mVertexBuffer.size() - 1;
    }

    void MeshStats::collect(FbxMesh* inFbxMesh)
    {
        check(inFbxMesh);
        clear();

        NumTotalPolygons = inFbxMesh->GetPolygonCount();
        NumTotalVertices = inFbxMesh->GetPolygonVertexCount();
        NumTotalControlPoints = inFbxMesh->GetControlPointsCount();

        getLayerInfo(MaterialLayerInfo, [&inFbxMesh]()->int{return inFbxMesh->GetElementMaterialCount(); }, [&inFbxMesh](int i)->FbxLayerElement*{return inFbxMesh->GetElementMaterial(i); });

        getLayerInfo(NormalLayerInfo, [&inFbxMesh]()->int{return inFbxMesh->GetElementNormalCount(); }, [&inFbxMesh](int i)->FbxLayerElement*{return inFbxMesh->GetElementNormal(i); });

        getLayerInfo(BinormalLayerInfo, [&inFbxMesh]()->int{return inFbxMesh->GetElementBinormalCount(); }, [&inFbxMesh](int i)->FbxLayerElement*{return inFbxMesh->GetElementBinormal(i); });

        getLayerInfo(TangentLayerInfo, [&inFbxMesh]()->int{return inFbxMesh->GetElementTangentCount(); }, [&inFbxMesh](int i)->FbxLayerElement*{return inFbxMesh->GetElementTangent(i); });

        getLayerInfo(UVLayerInfo, [&inFbxMesh]()->int{return inFbxMesh->GetElementUVCount(); }, [&inFbxMesh](int i)->FbxLayerElement*{return inFbxMesh->GetElementUV(i); });
    }

    void Mesh::parse(FbxNode* inNode)
    {
        check(inNode);
        check(hasAttr<FbxNodeAttribute::eMesh>(inNode));

        clear();

        mFbxNode = inNode;
        mFbxMesh = inNode->GetMesh();
        check(mFbxMesh);

        mNodeName = mFbxNode->GetName();
        mMeshName = mFbxMesh->GetName();

        extractMaterials();
        extractMeshGeometry();

        // Test
        std::string meshFootprint, fbxFootprint;
        getMemoryFootprint(&meshFootprint);
        getFBXMemoryFootprint(&fbxFootprint);
    }

    void Mesh::clear()
    {
        for (auto material : mMaterials)
        {
            if (material)
            {
                delete material;
                material = nullptr;
            }
        }
    }

    uint32_t Mesh::getMemoryFootprint(std::string* outString /*= nullptr*/) const
    {
        return mVertexStream.calcMemoryUsage(outString);
    }

    uint32_t Mesh::getFBXMemoryFootprint(std::string* outString /*= nullptr*/) const
    {
        uint32_t mem = (uint32_t)mFbxMesh->MemoryUsage();
        if (outString)
        {
            (*outString) += string_format("%.0f KB", (float)mem / 1024);
            (*outString) += string_format(" (%.2f MB)", (float)mem / 1024 / 1024);
        }
        return mem;
    }

    void Mesh::extractMaterials()
    {
        int numFbxMaterial = mFbxNode->GetMaterialCount();
        for (int fbxMaterialIndex = 0; fbxMaterialIndex < numFbxMaterial; ++fbxMaterialIndex)
        {
            FbxSurfaceMaterial* fbxMaterial = mFbxNode->GetMaterial(fbxMaterialIndex);
            check(fbxMaterial);
            Material* material = new Material(fbxMaterial);
            mMaterials.push_back(material);
        }
    }

    void Mesh::extractMeshGeometry()
    {
        // Get mesh stats
        mMeshStats.collect(mFbxMesh);


        // TODO Need to be tested with multi-materials mesh
        // fbxPolygonMaterialLink[polygon_Ind] gives an index into the material array.
        const FbxLayerElementArrayTemplate<int>& fbxPolygonMaterialLink = mFbxMesh->GetElementMaterial()->GetIndexArray();

        vcheck(mFbxNode->GetMaterialCount() == mMeshStats.MaterialLayerInfo.size(), L"Should equal");

        mVertexStream.preAllocate((uint32_t)mFbxNode->GetMaterialCount(), (uint32_t)mMeshStats.NumTotalVertices);
        // Mesh -> Polygon 
        int32_t pvIndex = 0;
        for (int32_t polyIndex = 0; polyIndex < mMeshStats.NumTotalPolygons; polyIndex++)
        {
            VertexSignature triangleSig[3];
            VertexSignature quadSig[4];
            Vertex triangle[3];
            Vertex quad[4];

            Vertex* polygon = nullptr;
            VertexSignature* signature = nullptr;

            // Get the number of polygon vertices in this polygon to determine polygon type
            int32_t sizeOfThisPolygon = mFbxMesh->GetPolygonSize(polyIndex);
            switch (sizeOfThisPolygon)
            {
            case 3:
                mMeshStats.Triangle_Counter++;
                polygon = triangle;
                signature = triangleSig;
                break;
            case 4:
                mMeshStats.Quad_Counter++;
                polygon = quad;
                signature = quadSig;
                break;

            default:
                vcheck(false, L"Unknown polygon type");
                polygon = triangle;
                signature = triangleSig;
                break;
            }

            // Polygon -> Vertex
            for (int32_t vertIndex = 0; vertIndex < sizeOfThisPolygon; ++vertIndex)
            {
                int32_t cpIndex = mFbxMesh->GetPolygonVertex(polyIndex, vertIndex);
                int32_t uvIndex = mFbxMesh->GetTextureUVIndex(polyIndex, vertIndex);

                polygon[vertIndex].readPosition(mFbxMesh, cpIndex, signature[vertIndex]);
                polygon[vertIndex].readNormal(mFbxMesh, cpIndex, pvIndex, signature[vertIndex]);
                polygon[vertIndex].readUV(mFbxMesh, cpIndex, uvIndex, signature[vertIndex]);

                pvIndex++;
            }

            int32_t materialIndex = fbxPolygonMaterialLink.GetAt(polyIndex);
            mVertexStream.addPolygon(polygon, signature, (uint32_t)sizeOfThisPolygon, (uint32_t)materialIndex);
        }
    }

    void Scene::parse(FbxScene* inScene)
    {
        check(inScene);
        clear();
        mFbxScene = inScene;

        extractSkeletons();
        extractMeshes();
    }

    void Scene::clear()
    {
        mFbxScene = nullptr;
        for (auto sk : mSkeletons)
        {
            delete sk;
            sk = nullptr;
        }
        for (auto mesh : mMeshes)
        {
            delete mesh;
            mesh = nullptr;
        }
    }

    void Scene::extractSkeletons()
    {
        check(mFbxScene);
        FbxNode* root = mFbxScene->GetRootNode();
        check(root);

        // FbxScene might contain more than one skeleton
        std::vector<FbxNode*> fbxSkeletonRoots;
        if (hasAttr<FbxNodeAttribute::eSkeleton>(root))
        {
            fbxSkeletonRoots.push_back(root);
        }
        else
        {
            searchSkeletonRoots(root, fbxSkeletonRoots);
        }

        // Now build each skeleton from respective skeleton root
        for (auto root : fbxSkeletonRoots)
        {
            Bone* rootBone = new Bone(root);
            buildSkeleton(rootBone);
            Skeleton* skeleton = new Skeleton(rootBone);

            // Debug stats
            uint32_t numBones = skeleton->getTotalBoneNum();
            std::string skeletonString;
            skeleton->toString(skeletonString);

            mSkeletons.push_back(skeleton);
        }

    }

    void Scene::extractMeshes()
    {
        check(mFbxScene);
        FbxNode* root = mFbxScene->GetRootNode();
        check(root);

        traverseFbx(root, [&](FbxNode* inNode)->void{
            if (hasAttr<FbxNodeAttribute::eMesh>(inNode))
            {
                mMeshes.push_back(new Mesh(inNode));
            }
        });
    }

}
