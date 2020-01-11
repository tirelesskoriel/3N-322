#ifndef DATA_WATCHER
#define DATA_WATCHER

#include <iostream>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h> 

#define LOG_PRIVATE(TAG, MSG) std::cout << (TAG) << " >>> " << (MSG) << std::endl;

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

int intResult{0};
aiColor3D c3{0.0f, 0.0f, 0.0f};
float floatResult{0.0f};
unsigned int max{1};

void processMaterialIntPros(const aiReturn* r, const char* tn)
{
    if(*r == AI_SUCCESS)
    {
        std::cout << tn << ":" << intResult;
        std::cout << "  " << max << std::endl;
    }
}

void processMaterialFloatPros(const aiReturn* r, const char* tn)
{
    if(*r == AI_SUCCESS)
    {
        std::cout << tn << ":" << floatResult;
        std::cout << "  " << max << std::endl;
    }
}

void processMaterial3DPros(const aiReturn* r, const char* tn)
{
    std::cout << "result: " << *r << std::endl;
    if(*r == AI_SUCCESS)
    {
        std::cout << tn << ":" << c3.r << " " << c3.b << " " << c3.g<< std::endl;
    }
}

void processMaterialPros(const aiMaterial* material)
{
    aiReturn r;
    r = material->Get(AI_MATKEY_TWOSIDED,&intResult, &max);
    processMaterialIntPros(&r, "AI_MATKEY_TWOSIDED");

    r = material->Get(AI_MATKEY_SHADING_MODEL,&intResult, &max);
    processMaterialIntPros(&r, "AI_MATKEY_SHADING_MODEL");

    r = material->Get(AI_MATKEY_ENABLE_WIREFRAME,&intResult, &max);
    processMaterialIntPros(&r, "AI_MATKEY_ENABLE_WIREFRAME");

    r = material->Get(AI_MATKEY_BLEND_FUNC,&intResult, &max);
    processMaterialIntPros(&r, "AI_MATKEY_BLEND_FUNC");

    r = material->Get(AI_MATKEY_OPACITY,&floatResult, &max);
    processMaterialFloatPros(&r, "AI_MATKEY_OPACITY");

    r = material->Get(AI_MATKEY_SHININESS,&floatResult, &max);
    processMaterialFloatPros(&r, "AI_MATKEY_SHININESS");
    
    r = material->Get(AI_MATKEY_REFLECTIVITY,&floatResult, &max);
    processMaterialFloatPros(&r, "AI_MATKEY_REFLECTIVITY");

    r = material->Get(AI_MATKEY_SHININESS_STRENGTH,&floatResult, &max);
    processMaterialFloatPros(&r, "AI_MATKEY_SHININESS_STRENGTH");
    
    r = material->Get(AI_MATKEY_REFRACTI,&floatResult, &max);
    processMaterialFloatPros(&r, "AI_MATKEY_REFRACTI");
    
    r = material->Get(AI_MATKEY_COLOR_DIFFUSE,c3);
    processMaterial3DPros(&r, "AI_MATKEY_COLOR_DIFFUSE");

    r = material->Get(AI_MATKEY_COLOR_AMBIENT,c3);
    processMaterial3DPros(&r, "AI_MATKEY_COLOR_AMBIENT");
    
    r = material->Get(AI_MATKEY_COLOR_SPECULAR,c3);
    processMaterial3DPros(&r, "AI_MATKEY_COLOR_SPECULAR");

    r = material->Get(AI_MATKEY_COLOR_EMISSIVE,c3);
    processMaterial3DPros(&r, "AI_MATKEY_COLOR_EMISSIVE");

    r = material->Get(AI_MATKEY_COLOR_TRANSPARENT,c3);
    processMaterial3DPros(&r, "AI_MATKEY_COLOR_TRANSPARENT");

    r = material->Get(AI_MATKEY_COLOR_REFLECTIVE,c3);
    processMaterial3DPros(&r, "AI_MATKEY_COLOR_REFLECTIVE");

}

bool DoTheImportThing(const aiScene* scene)
{
    // if(scene->HasTextures())
    // {
    //     LOG_PRIVATE("TEXTURE","-----");
    //     for(int i = 0; i < scene->mNumTextures; i++)
    //     {
    //         std::cout << "hint:" << scene->mTextures[i]->achFormatHint;
    //         std::cout << " WIDTH:" << scene->mTextures[i]->mWidth;
    //         std::cout << " HEIGHT:" << scene->mTextures[i]->mHeight;
    //         std::cout << " FILE:" << scene->mTextures[i]->mFilename.C_Str() << std::endl;
    //     }
    // }
    // if(scene->HasMeshes())
    // {   
    //     LOG_PRIVATE("MESH","-----");
    //     for(int i = 0; i < scene->mNumMeshes; i++)
    //     {
    //         aiMesh* mesh = scene->mMeshes[i];
    //         if(mesh->HasBones())
    //         {
    //             LOG_PRIVATE_PRIVATE("MESH",i);
    //             for (int m = 0; m < mesh->mNumBones; m++)
    //             {
    //                 aiBone* bone = mesh->mBones[m];
    //                 std::cout << "name:" << bone->mName.C_Str();
    //                 std::cout << " size:" << bone->mNumWeights<< std::endl;

    //                 for(int k = 0; k < bone->mNumWeights; k++)
    //                 {
    //                     std::cout << " mVertexId:" << bone->mWeights[k].mVertexId;
    //                     std::cout << " mWeight:" << bone->mWeights[k].mWeight<< std::endl;
    //                 }
    //             }
    //         }
            
    //     }
    // }

    if(scene->HasMaterials())
    {   
        LOG_PRIVATE("MATERIALS","-----");
        for (int i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial* material = scene->mMaterials[i];
            std::cout << " FILE:" << material->GetName().C_Str() << std::endl;

            processMaterialPros(material);
            
            // LOG_PRIVATE("MATERIALS","pros ------");
            // for(int j = 0; j < material->mNumProperties; j++)
            // {
            //     aiMaterialProperty* mp = material->mProperties[j];
            //     std::cout << "properites key:" << mp->mKey.C_Str();
            //     std::cout << "  semantic:" << mp->mSemantic << std::endl;
            //     if(mp->mData)
            //     {
            //         std::cout << "  exit:" << mp->mData;
            //         std::cout << "  index:" << mp->mIndex;
            //         std::cout << "  type:" << mp->mType;
            //         std::cout << "  length:"<< mp->mDataLength << std::endl;
            //     }
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
    return true;
}

#endif