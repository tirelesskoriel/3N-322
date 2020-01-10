#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <tools/my_shader_loader.h>
#include <string>
#include <vector>

const int MAX_POINT = 4;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    // glm::vec2 TexCoords;
    // glm::vec3 TexColor;
    // glm::vec3 Tangent;
    // glm::vec3 Bitangent;
    float weight[MAX_POINT] = {0.0f};
    unsigned int boneId[MAX_POINT] = {0};

    void add(float w, uint id);

    bool is_init();
};

struct Bone{
    unsigned int id;
    aiString name;
    aiMatrix4x4 offset;
    aiMatrix4x4 finalOffset;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    float shininess;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float shininess);
    void Draw(ShaderLoader shader);

private:
    unsigned int VBO, EBO;
    void setupMesh();
};
#endif
