
#ifndef CUSTOM_MATH
#define CUSTOM_MATH

#include <glm/glm.hpp>
#include <assimp/scene.h>

glm::mat4 transByMat4(const aiMatrix4x4* aim4)
{
    glm::mat4 mat4;
    mat4[0][0] = aim4->a1;  mat4[0][1] = aim4->a2;  mat4[0][2] = aim4->a3;  mat4[0][3] = aim4->a4;
    mat4[1][0] = aim4->b1;  mat4[1][1] = aim4->b2;  mat4[1][2] = aim4->b3;  mat4[1][3] = aim4->b4;
    mat4[2][0] = aim4->c1;  mat4[2][1] = aim4->c2;  mat4[2][2] = aim4->c3;  mat4[2][3] = aim4->c4;
    mat4[3][0] = aim4->d1;  mat4[3][1] = aim4->d2;  mat4[3][2] = aim4->d3;  mat4[3][3] = aim4->d4;
    return mat4;
}

void transByMat4(glm::mat4* mat4, const aiMatrix4x4* aim4)
{
    (*mat4)[0][0] = aim4->a1;  (*mat4)[0][1] = aim4->a2;  (*mat4)[0][2] = aim4->a3;  (*mat4)[0][3] = aim4->a4;
    (*mat4)[1][0] = aim4->b1;  (*mat4)[1][1] = aim4->b2;  (*mat4)[1][2] = aim4->b3;  (*mat4)[1][3] = aim4->b4;
    (*mat4)[2][0] = aim4->c1;  (*mat4)[2][1] = aim4->c2;  (*mat4)[2][2] = aim4->c3;  (*mat4)[2][3] = aim4->c4;
    (*mat4)[3][0] = aim4->d1;  (*mat4)[3][1] = aim4->d2;  (*mat4)[3][2] = aim4->d3;  (*mat4)[3][3] = aim4->d4;
}

glm::mat4 initScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
    glm::mat4 m;
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
    return m;
}

glm::mat4 transByMat3(const aiMatrix3x3& AssimpMatrix)
{
    glm::mat4 m;
    m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = 0.0f;
    m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = 0.0f;
    m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = 0.0f;
    m[3][0] = 0.0f           ; m[3][1] = 0.0f           ; m[3][2] = 0.0f           ; m[3][3] = 1.0f;
    return m;
} 

glm::mat4 initTranslationTransform(float x, float y, float z)
{
    glm::mat4 m;
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    return m;
}
glm::mat4 init()
{
    glm::mat4 m;
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    return m;
}

#endif