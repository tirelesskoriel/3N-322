#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aWeight1;
layout (location = 3) in vec4 aWeight2;
layout (location = 4) in ivec4 aBoneId1;
layout (location = 5) in ivec4 aBoneId2;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 nor_model;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];


void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = nor_model * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}