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
#include <sys/time.h>
#include <model/custom_math.h>
#include <tools/syntax_sugar.h>
#include <tools/data_watcher.h>
#include <cmath>

Model::Model(std::string const &path, bool auto_size) : auto_size(auto_size)
{
    shader = new ShaderLoader("animation.vs", "model.fs");
    loadModel(path);
    if(auto_size)
        auto_scale_value = AUTO_SIZE_SCALING_RATIO / fmaxf(fmaxf((max_x - min_x), (max_y - min_y)), (max_z - min_z));
}

Model::~Model()
{
    for (int i = 0; i < meshes.size(); i++)
    {
        SAVE_DEL(meshes[i]);
    }

    delete shader;
}

void Model::Draw(glm::mat4& model)
{
    shader->use();
    if(auto_size)
        model = glm::scale(model, glm::vec3(auto_scale_value, auto_scale_value, auto_scale_value));
    
    shader->setMat4("model", model);
    shader->setMat3("nor_model", glm::mat3(glm::transpose(glm::inverse(model))));

    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i]->Draw(shader);
}

void Model::runAnimator()
{
    if(!scene->HasAnimations())
        return;
        
    float RunningTime = GetRunningTime();
    BoneTransform(RunningTime, Transforms);
    for (uint i = 0 ; i < Transforms.size() && i < 100 ; i++) {
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, boneLocations[i].c_str()), 1, GL_TRUE, &Transforms[i][0][0]);
    }
}

bool Model::hasAnimation()
{
    return scene && scene->HasAnimations();
};

void Model::loadModel(std::string const &path)
{
    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // DoTheImportThing(scene);

    directory = path.substr(0, path.find_last_of('/'));

    m_GlobalInverseTransform = scene->mRootNode->mTransformation;
    m_GlobalInverseTransform.Inverse();
    
    if(scene->HasAnimations())
    {
        m_startTime = GetCurrentTimeMillis();
        for (int i = 0; i < 100; i++)
        {
            std::stringstream ss;
            ss << "gBones[" << i << "]";
            boneLocations.push_back(ss.str());
        }
    }

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

        max_x = fmaxf(max_x, vector.x);
        max_y = fmaxf(max_y, vector.y);
        max_z = fmaxf(max_z, vector.z);

        min_x = fminf(min_x, vector.x);
        min_y = fminf(min_y, vector.y);
        min_z = fminf(min_z, vector.z);

        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;

            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        

        if(mesh->mTangents)
        {
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
        }

        if(mesh->mBitangents)
        {
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }

        for(int j = 0 ; j < AI_MAX_NUMBER_OF_COLOR_SETS; j++)
        {
            if(mesh->HasVertexColors(j))
            {
                vector.x = mesh->mColors[j][i].r;
                vector.y = mesh->mColors[j][i].g;
                vector.z = mesh->mColors[j][i].b;
                vertex.TexColor = vector;
            }
        }
        
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

    std::vector<Texture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
    textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

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

    unsigned int baseIndex = bones.size();
    for (unsigned int m = 0; m < mesh->mNumBones; m++)
    {
        aiBone* aiBone = mesh->mBones[m];
        Bone bone;
        bone.id = m + baseIndex;
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
        int r = TextureFromFile(str.C_Str(), this->directory);
        if(!skip && !IS_ERROR(r))
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
    std::string filename = REPLACE(path, '\\', '/');
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
        return textureID;
    }
    else
    {
        WARNING_LOG("Texture failed to load at path: ", path);
        stbi_image_free(data);
        return COMMON_ERROR_CODE;
    }

    
}

long long Model::GetCurrentTimeMillis()
{
    timeval t;
    gettimeofday(&t, NULL);

    long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    return ret;
}

float Model::GetRunningTime()
{
    float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;
    return RunningTime;
}

unsigned int Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    // assert(0);
    return 0;
}


unsigned int Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    // assert(0);
    return 0;
}


unsigned int Model::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    // assert(0);
    return 0;
}


bool Model::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return true;
    }
            
    uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    uint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    // assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;

    return Factor >= 0.0f && Factor <= 1.0f;
}


bool Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return true;
    }
    
    uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    uint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    // assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();

    return Factor >= 0.0f && Factor <= 1.0f;
}


bool Model::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return true;
    }

    uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    // assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;

    return Factor >= 0.0f && Factor <= 1.0f;
}


void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
{    
    const aiAnimation* pAnimation = scene->mAnimations[0];
        
    aiMatrix4x4 NodeTransformation(pNode->mTransformation);
    // transByMat4(&NodeTransformation, &pNode->mTransformation);
     
    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, pNode->mName);
    
    bool skip = false;
    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        skip = !CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        aiMatrix4x4 ScalingM = initScaleTransformAI(Scaling.x, Scaling.y, Scaling.z);
        
        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        skip = !CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);        
        aiMatrix4x4 RotationM = transByMat3AI(RotationQ.GetMatrix());

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        skip = !CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        aiMatrix4x4 TranslationM = initTranslationTransformAI(Translation.x, Translation.y, Translation.z);
        
        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }
    
    aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;
    for (unsigned int i = 0 ; i < bones.size() ; i++) {
        if (bones[i].name == pNode->mName)
        {
            bones[i].finalOffset = GlobalTransformation * (bones[i].offset);
            bones[i].skipTime = skip;
        }
    }
    
    for (unsigned int i = 0 ; i < pNode->mNumChildren ; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}


void Model::BoneTransform(float TimeInSeconds, std::vector<aiMatrix4x4>& Transforms)
{
    // glm::mat4 Identity = init();
    aiMatrix4x4 Identity = initAI();
    
    float TicksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)scene->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(AnimationTime, scene->mRootNode, Identity);
    Transforms.resize(bones.size());

    bool is_error = false;
    for (unsigned int i = 0 ; i < bones.size() ; i++) {
        if(bones[i].skipTime)
        {
            is_error = true;
            continue;
        };
        Transforms[i] = bones[i].finalOffset;
    }

    if (is_error)
        WARNING_LOG("ERROR FRAME! ", AnimationTime);
}


const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, const aiString NodeName)
{
    for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
        if (pNodeAnim->mNodeName == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}