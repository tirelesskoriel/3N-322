#include <model/model.h> 
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdimage/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <model/mesh.h>
#include <tools/my_shader_loader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

Model::Model(std::string const &path, bool gamma) : gammaCorrection(gamma)
{
    loadModel(path);
}

Model::~Model()
{
    for (int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
}

void Model::Draw(ShaderLoader shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i]->Draw(shader);
}

void Model::loadModel(std::string const &path)
{
    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    m_GlobalInverseTransform = scene->mRootNode->mTransformation;
    m_GlobalInverseTransform.Inverse();
    
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);

}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // texture coordinates
        // if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        // {
        //     glm::vec2 vec;
        //     // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
        //     // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
        //     vec.x = mesh->mTextureCoords[0][i].x; 
        //     vec.y = mesh->mTextureCoords[0][i].y;

        //     vertex.TexCoords = vec;
        // }
        // else
        //     vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        

        // if(mesh->mTangents)
        // {
        //     // tangent
        //     vector.x = mesh->mTangents[i].x;
        //     vector.y = mesh->mTangents[i].y;
        //     vector.z = mesh->mTangents[i].z;
        //     vertex.Tangent = vector;
        // }

        // if(mesh->mBitangents)
        // {
        //     // bitangent
        //     vector.x = mesh->mBitangents[i].x;
        //     vector.y = mesh->mBitangents[i].y;
        //     vector.z = mesh->mBitangents[i].z;
        //     vertex.Bitangent = vector;
        // }

        // for(int j = 0 ; j < AI_MAX_NUMBER_OF_COLOR_SETS; j++)
        // {
        //     if(mesh->HasVertexColors(j))
        //     {
        //         vector.x = mesh->mColors[j][i].r;
        //         vector.y = mesh->mColors[j][i].g;
        //         vector.z = mesh->mColors[j][i].b;
        //         vertex.TexColor = vector;
        //     }
        // }
        
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    aiColor3D c3;
    aiReturn r;

    r = material->Get(AI_MATKEY_COLOR_DIFFUSE,c3);
    if(r == AI_SUCCESS)
    {
        diffuse.r = c3.r;
        diffuse.g = c3.g;
        diffuse.b = c3.b;
    }
    r = material->Get(AI_MATKEY_COLOR_SPECULAR,c3);
    if(r == AI_SUCCESS)
    {
        specular.r = c3.r;
        specular.g = c3.g;
        specular.b = c3.b;
    }
    r = material->Get(AI_MATKEY_COLOR_AMBIENT,c3);
    if(r == AI_SUCCESS)
    {
        ambient.r = c3.r;
        ambient.g = c3.g;
        ambient.b = c3.b;
    }

    float s = 1;
    material->Get(AI_MATKEY_SHININESS, &s, NULL);

    for (unsigned int m = 0; m < mesh->mNumBones; m++)
    {
        aiBone* aiBone = mesh->mBones[m];
        Bone bone;
        bone.id = m;
        bone.offset = aiBone->mOffsetMatrix;
        bone.name = aiBone->mName;
        bones.push_back(bone);
        for (unsigned int k = 0; k < aiBone->mNumWeights; k++)
        {
            vertices[aiBone->mWeights[k].mVertexId].add(aiBone->mWeights[k].mWeight, bone.id);
        }
    }

    return new Mesh(vertices, indices, textures, diffuse, specular, ambient, s);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}