#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader/my_shader_loader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

const int MAX_POINT = 4;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // // texCoords
    // glm::vec2 TexCoords;

    // glm::vec3 TexColor;
    // // tangent
    // glm::vec3 Tangent;
    // // bitangent
    // glm::vec3 Bitangent;

    float weight[MAX_POINT] = {0.0f};
    uint boneId[MAX_POINT] = {0};

    void add(float w, int id)
    {
        if (w <= 0.0f) return;

        for (uint i = 0; i < MAX_POINT; i++)
        {
            if (weight[i] == w && boneId[i] == id)
            {
                return;
            }
        }

        for (uint i = 0; i < MAX_POINT; i++)
        {
            if(weight[i] == 0.0f)
            {
                weight[i] = w;
                boneId[i] = id;
                break;
            }
        }
    }

    bool is_init()
    {
        return weight[0] == 0.0f && boneId[0] == 0;
    }
};

struct Bone{
    unsigned int id;
    aiString name;
    glm::mat4 offset;
    glm::mat4 finalOffset;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    float shininess;

    unsigned int VAO;

    /*  Functions  */
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float shininess)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->ambient = ambient;
        this->specular = specular;
        this->diffuse = diffuse;
        this->shininess = shininess;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(ShaderLoader shader) 
    {
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if(name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if(name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if(name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
             else if(name == "texture_height")
			    number = std::to_string(heightNr++); // transfer unsigned int to stream

													 // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        shader.use();
        shader.setVec3("material.diffuse", diffuse);
        shader.setVec3("material.ambient", ambient);
        shader.setVec3("material.specular", specular);
        shader.setFloat("material.shininess", shininess);
        
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    /*  Render data  */
    unsigned int VBO, EBO;

    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        // glEnableVertexAttribArray(2);	
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // // vertex texture colors
        // glEnableVertexAttribArray(3);	
        // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexColor));
        // // vertex tangent
        // glEnableVertexAttribArray(4);
        // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // // vertex bitangent
        // glEnableVertexAttribArray(5);
        // glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        // Vertex vd;
        std::cout<< "bind-------" <<std::endl;

        for (unsigned int k = 0; k < this->vertices.size(); k++)
        {
            if (!this->vertices[k].is_init())
            {
                std::cout << "w1:" << this->vertices[k].weight[0]<< "   b1:" << this->vertices[k].boneId[0];
                std::cout << "      w2:" << this->vertices[k].weight[1]<< "  b2:" << this->vertices[k].boneId[1];
                std::cout << "      w3:" << this->vertices[k].weight[2]<< "  b3:" << this->vertices[k].boneId[2];
                std::cout << "      w4:" << this->vertices[k].weight[3]<< "  b4:" << this->vertices[k].boneId[3] << std::endl;
            }
        }

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, MAX_POINT, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weight));

        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, MAX_POINT, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneId));

        glBindVertexArray(0);
    }
};
#endif
