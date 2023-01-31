#include "Model.h"
#include "glm/fwd.hpp"
#include "Texture.h"
#include "runtime/platform/path_utility/PathUtility.h"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <iostream>
#include <string>
#include <vector>
namespace EasyEngine{
    using namespace std;
        
        void Model::loadModel(const string& path){
            Assimp::Importer importer;
            //transform all the model's primitive shapeds to triangles first.
            //filps the texture coordinates on the y-axis
            const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                cout<< "ERROR::ASSIMP::"<<importer.GetErrorString()<<endl;
                return;
            }
            directory = path.substr(0,path.find_last_of('/'));

            processNode(scene->mRootNode, scene);
        }

        void Model::draw(Shader &shader){

            for (Mesh mesh : meshes) {
                mesh.draw(shader);
            }
        }

        void Model::processNode(aiNode *node,const aiScene *scene){
                        // for (int i = 0; i<meshes.size(); ++i) {
                cout<<loadedCount++<<"/"<<scene->mNumMeshes<<endl;
            // }
            for (unsigned int i = 0; i<node->mNumMeshes; ++i) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.emplace_back(processMesh(mesh,scene));
            }
            for (int i = 0; i<node->mNumChildren; ++i) {
                processNode(node->mChildren[i], scene);
            }
        }
        Mesh Model::processMesh(aiMesh *mesh,const aiScene *scene){
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Texture> textures;

            for (unsigned int i = 0; i<mesh->mNumVertices; ++i) {
                Vertex vertex;
                vertex.position = glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
                vertex.normal = glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
                if (mesh->mTextureCoords[0]) {
                    vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
                }
                else {
                    vertex.texCoords = glm::vec2(0);
                }
                vertices.emplace_back(vertex);            
            }
            for (unsigned int i = 0; i<mesh->mNumFaces; ++i) {
                const aiFace& face = mesh->mFaces[i];
                for (unsigned int j = 0; j<face.mNumIndices; ++j) {
                    indices.emplace_back(face.mIndices[j]);
                }
            }
            if (mesh->mMaterialIndex >=0) {
                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
                vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(),specularMaps.end());
            }
            return Mesh(vertices,indices,textures);
        }
        vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,aiTextureType type,string typeName){
            vector<Texture> textures;
            for (uint i = 0; i<mat->GetTextureCount(type); ++i) {
                aiString str;
                mat->GetTexture(type, i, &str);
                bool skip = false;
                for(const Texture& tex:textures_loaded){
                    if (strcmp(tex.path.data(),str.C_Str())==0) {
                        textures.emplace_back(tex);
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
                Texture texture(PathUtility::getFullPath(directory, str.C_Str()));                
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.emplace_back(texture);
                textures_loaded.emplace_back(texture);
            }
            return textures;
        }

        Model::Model(const string& path){
            loadModel(path);
            loadedCount = 0;
        }

}