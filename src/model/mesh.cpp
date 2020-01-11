#include <model/mesh.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tools/my_shader_loader.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <tools/syntax_sugar.h>

void Vertex::add(float w, unsigned int id)
{
    for (unsigned int i = 0; i < MAX_POINT; i++)
    {
        if(weight[i] == 0.0f)
        {
            weight[i] = w;
            boneId[i] = id;
            break;
        }
    }
}

bool Vertex::is_init()
{
    return weight[0] == 0.0f && boneId[0] == 0;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float shininess)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->ambient = ambient;
    this->specular = specular;
    this->diffuse = diffuse;
    this->shininess = shininess;

    setupMesh();
}

void Mesh::Draw(const ShaderLoader* shader) 
{
    unsigned int diffuseNr  = 1;
    unsigned int ambientNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string pre("material.");
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        if(name == "texture_ambient")
            number = std::to_string(ambientNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_normal")
            number = std::to_string(normalNr++);
        else if(name == "texture_height")
            number = std::to_string(heightNr++);

        glUniform1i(glGetUniformLocation(shader->ID, (pre + name + number).c_str()), i);
        // LOG((pre + name + number).c_str());
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    shader->use();
    shader->setVec3("material.diffuse", diffuse);
    shader->setVec3("material.ambient", ambient);
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
    shader->setBool("hasTexture", textures.size() > 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    // Vertex vd;
    // std::cout<< "bind-------" <<std::endl;

    // for (unsigned int k = 0; k < this->vertices.size(); k++)
    // {
    //     if (!this->vertices[k].is_init())
    //     {
    //         std::cout << "w1:" << this->vertices[k].weight[0]<< "   b1:" << this->vertices[k].boneId[0];
    //         std::cout << "      w2:" << this->vertices[k].weight[1]<< "  b2:" << this->vertices[k].boneId[1];
    //         std::cout << "      w3:" << this->vertices[k].weight[2]<< "  b3:" << this->vertices[k].boneId[2];
    //         std::cout << "      w4:" << this->vertices[k].weight[3]<< "  b4:" << this->vertices[k].boneId[3];
    //         std::cout << "      w5:" << this->vertices[k].weight[4]<< "  b5:" << this->vertices[k].boneId[4];
    //         std::cout << "      w6:" << this->vertices[k].weight[5]<< "  b6:" << this->vertices[k].boneId[5];
    //         std::cout << "      w7:" << this->vertices[k].weight[6]<< "  b7:" << this->vertices[k].boneId[6];
    //         std::cout << "      w8:" << this->vertices[k].weight[7]<< "  b8:" << this->vertices[k].boneId[7] << std::endl;
    //     }
    // }

    // for (unsigned int k = 0; k < this->vertices.size(); k++)
    // {
    //     std::cout << "v1:" << this->vertices[k].TexCoords[0] << "   v2:" << this->vertices[k].TexCoords[1] << std::endl;
    // }
 
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, MAX_POINT/2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weight));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, MAX_POINT/2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, weight) + (offsetof(Vertex, boneId) - offsetof(Vertex, weight)) / 2));

    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, MAX_POINT/2, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneId));

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, MAX_POINT/2, GL_INT, sizeof(Vertex), (void*)(offsetof(Vertex, boneId) + (sizeof(Vertex) - offsetof(Vertex, boneId)) / 2));

    glEnableVertexAttribArray(6);	
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    
    glEnableVertexAttribArray(7);	
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexColor));
    
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    
    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    
    glBindVertexArray(0);
}