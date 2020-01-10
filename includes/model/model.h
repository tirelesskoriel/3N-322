#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <model/mesh.h>
#include <tools/my_shader_loader.h>

#include <string>
#include <vector>

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

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

    Model(std::string const &path, bool gamma = false);
    ~Model();
    void Draw(ShaderLoader shader);
    
private:
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);
};
#endif
