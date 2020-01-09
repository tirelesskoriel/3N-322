#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTexColor;
layout (location = 6) in vec4 aWeight;
layout (location = 7) in ivec4 aBoneId;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 TexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 nor_model;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

void main()
{
    mat4 BoneTransform = gBones[aBoneId[0]] * aWeight[0];
    BoneTransform     += gBones[aBoneId[1]] * aWeight[1];
    BoneTransform     += gBones[aBoneId[2]] * aWeight[2];
    BoneTransform     += gBones[aBoneId[3]] * aWeight[3];

    TexCoords = aTexCoords;  
    TexColor = aTexColor;  


    
    FragPos = vec3(model * vec4(aPos, 1.0));
    // FragPos = vec3(model * (BoneTransform * vec4(aPos, 1.0)));
    Normal = nor_model * aNormal;  
    gl_Position = BoneTransform * projection * view * model * vec4(aPos, 1.0);
}