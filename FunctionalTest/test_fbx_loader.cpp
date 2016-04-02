#include "catch.hpp"

#include "Core/FbxLoader.h"

TEST_CASE("Hand roll FBX loader", "[Core]") {

    SECTION("load a mesh") {
        FbxLoader::getInstance()->initialize();

        FbxInput input;
        input.FilePath = ws2s(FileOp::getDefaultPath<FileOp::MeshResource>().toString());
        input.Filename = "sword_and_shield_slash_2.fbx";
        //input.Filename = "jogging_1.fbx";
        //input.Filename = "t-pose_19.fbx";
        //input.Filename = "opening_a_filing_cabinet_1.fbx";
        input.RemoveDuplicatedVertex = true;

        FbxOutput output;

        FbxLoader::getInstance()->load(input, output);
    }
}