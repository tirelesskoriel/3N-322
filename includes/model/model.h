#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <model/mesh.h>
#include <tools/my_shader_loader.h>
#include <string>
#include <vector>

class Model 
{
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh*> meshes;
    std::vector<Bone> bones;
    const aiScene* scene;
    Assimp::Importer importer;
    std::string directory;
    bool gammaCorrection;
    aiMatrix4x4 m_GlobalInverseTransform;
    long long m_startTime;
    std::vector<aiMatrix4x4> Transforms;
    std::vector<std::string> boneLocations;

    Model(std::string const &path, bool gamma = false);
    ~Model();
    void Draw(ShaderLoader shader);
    void runAnimator(ShaderLoader shader);
    
private:
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);

    long long GetCurrentTimeMillis();
    float GetRunningTime();
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
    void BoneTransform(float TimeInSeconds, std::vector<aiMatrix4x4>& Transforms);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const aiString NodeName);
};
#endif
