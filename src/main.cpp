#include <iostream>
#include <filesystem.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h> 

#define LOG(TAG, MSG) std::cout << (TAG) << " >>> " << (MSG) << std::endl;

void getTextures(const aiMaterial* material, aiTextureType type, const char* tn)
{
    std::cout << " type name:" << tn << std::endl;
    
    for(int k = 0; k < material->GetTextureCount(type); k++)
    {
        aiString p;
        material->GetTexture(type, k, &p);
        std::cout << " path:" << p.C_Str() << std::endl;
        
    }
}

bool DoTheImportThing(const std::string& pFile)
{
    LOG("PATH", pFile);
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( pFile,
    aiProcess_CalcTangentSpace       |
    aiProcess_Triangulate            |
    aiProcess_JoinIdenticalVertices  |
    aiProcess_SortByPType);

    // If the import failed, report it
    if( !scene)
    {
        std::cout << "error:" << importer.GetErrorString() << std::endl;
        return false;
    }
    if(scene->HasTextures())
    {
        LOG("TEXTURE","-----");
        for(int i = 0; i < scene->mNumTextures; i++)
        {
            std::cout << "hint:" << scene->mTextures[i]->achFormatHint;
            std::cout << " WIDTH:" << scene->mTextures[i]->mWidth;
            std::cout << " HEIGHT:" << scene->mTextures[i]->mHeight;
            std::cout << " FILE:" << scene->mTextures[i]->mFilename.C_Str() << std::endl;
        }
    }
    if(scene->HasMeshes())
    {   
        LOG("MESH","-----");
    }

    if(scene->HasMaterials())
    {   
        LOG("MATERIALS","-----");
        for (int i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial* material = scene->mMaterials[i];
            std::cout << " FILE:" << material->GetName().C_Str() << std::endl;

            // for(int j = 0; j < material->mNumProperties; j++)
            // {
            //     aiMaterialProperty* mp = material->mProperties[j];
            //     std::cout << "properites key:" << mp->mKey.C_Str();
            //     std::cout << "  semantic:" << mp->mSemantic << std::endl;
            // }

            getTextures(material, aiTextureType::aiTextureType_AMBIENT, "aiTextureType_AMBIENT");
            getTextures(material, aiTextureType::aiTextureType_DIFFUSE, "aiTextureType_DIFFUSE");
            getTextures(material, aiTextureType::aiTextureType_DISPLACEMENT, "aiTextureType_DISPLACEMENT");
            getTextures(material, aiTextureType::aiTextureType_EMISSIVE, "aiTextureType_EMISSIVE");
            getTextures(material, aiTextureType::aiTextureType_HEIGHT, "aiTextureType_HEIGHT");
            getTextures(material, aiTextureType::aiTextureType_LIGHTMAP, "aiTextureType_LIGHTMAP");
            getTextures(material, aiTextureType::aiTextureType_NORMALS, "aiTextureType_NORMALS");
            getTextures(material, aiTextureType::aiTextureType_OPACITY, "aiTextureType_OPACITY");
            getTextures(material, aiTextureType::aiTextureType_REFLECTION, "aiTextureType_REFLECTION");
            getTextures(material, aiTextureType::aiTextureType_SHININESS, "aiTextureType_SHININESS");
            getTextures(material, aiTextureType::aiTextureType_SPECULAR, "aiTextureType_SPECULAR");
        }
    }
    

    // Now we can access the file's contents.
    // DoTheSceneProcessing( scene);
    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

int main()
{
    DoTheImportThing(FileSystem::getPath("resources/objects/gd/sazabi_1.obj").c_str());
    return 0;
}