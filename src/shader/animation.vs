#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aWeight1;
layout (location = 3) in vec4 aWeight2;
layout (location = 4) in ivec4 aBoneId1;
layout (location = 5) in ivec4 aBoneId2;

layout (location = 6) in vec2 aTexCoords;
layout (location = 7) in vec3 aTexColor;
layout (location = 8) in vec3 aTangent;
layout (location = 9) in vec3 aBitangent;


out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 TexColor;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 nor_model;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

void main()
{
    mat4 BoneTransform = gBones[aBoneId1[0]] * aWeight1[0];
    BoneTransform     += gBones[aBoneId1[1]] * aWeight1[1];
    BoneTransform     += gBones[aBoneId1[2]] * aWeight1[2];
    BoneTransform     += gBones[aBoneId1[3]] * aWeight1[3];

    BoneTransform     += gBones[aBoneId2[0]] * aWeight2[0];
    BoneTransform     += gBones[aBoneId2[1]] * aWeight2[1];
    BoneTransform     += gBones[aBoneId2[2]] * aWeight2[2];
    BoneTransform     += gBones[aBoneId2[3]] * aWeight2[3];

    FragPos = vec3(model * (BoneTransform * vec4(aPos, 1.0)));
    Normal = nor_model * aNormal;
    gl_Position = projection * view * model * (BoneTransform * vec4(aPos, 1.0));

    TexCoords = aTexCoords;
    TexColor = aTexColor;
    Tangent = aTangent;
    Bitangent = aBitangent;
}