#pragma once
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <map>
#include <string>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "glm/fwd.hpp"
#include "runtime/include/Shader.h"
#include "runtime/resource/res_type/components/Mesh.h"

namespace EasyEngine {
static int loadedCount = 0;

struct BoneInfo{
    int id;
    glm::mat4 offset;
};

class Model{
    public:
        void loadModel(const string& path);
        void processNode(aiNode *node,const aiScene *scene);
        Mesh processMesh(aiMesh *mesh,const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat,aiTextureType type,string typeName);
        Model(const string& path);
        void draw(Shader &shader);
        
        //animation
        auto& getBoneInfoMap(){
            return boneInfoMap;
        }
        int& getBoneCount();
        void setVertexBoneDataToDefault(Vertex& vertex);
        void setVertexBoneData(Vertex& vertex, int boneID, float weight);
        void extractBoneWeightForVertices(std::vector<Vertex>& vertices,aiMesh* mesh,const aiScene* scene);
    public:
        vector<Texture> textures_loaded; 
        vector<Mesh> meshes;
        string directory;
        //todo MAX_BONE_WEIGHTS
    private:
        //animation variable
        std::map<string, BoneInfo> boneInfoMap;
        int boneCount = 0;
};

}