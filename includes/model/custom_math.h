
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
    // return glm::transpose(mat4);
    return mat4;
}

void transByMat4(glm::mat4* mat4, const aiMatrix4x4* aim4)
{
    (*mat4)[0][0] = aim4->a1;  (*mat4)[0][1] = aim4->a2;  (*mat4)[0][2] = aim4->a3;  (*mat4)[0][3] = aim4->a4;
    (*mat4)[1][0] = aim4->b1;  (*mat4)[1][1] = aim4->b2;  (*mat4)[1][2] = aim4->b3;  (*mat4)[1][3] = aim4->b4;
    (*mat4)[2][0] = aim4->c1;  (*mat4)[2][1] = aim4->c2;  (*mat4)[2][2] = aim4->c3;  (*mat4)[2][3] = aim4->c4;
    (*mat4)[3][0] = aim4->d1;  (*mat4)[3][1] = aim4->d2;  (*mat4)[3][2] = aim4->d3;  (*mat4)[3][3] = aim4->d4;
    // glm::transpose(*mat4);
}

glm::mat4 initScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
    glm::mat4 m;
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
    // return glm::transpose(m);
    return m;
}
aiMatrix4x4 initScaleTransformAI(float ScaleX, float ScaleY, float ScaleZ)
{
    aiMatrix4x4 m;
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
    // return glm::transpose(m);
    return m;
}

glm::mat4 transByMat3(const aiMatrix3x3& AssimpMatrix)
{
    glm::mat4 m;
    m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = 0.0f;
    m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = 0.0f;
    m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = 0.0f;
    m[3][0] = 0.0f           ; m[3][1] = 0.0f           ; m[3][2] = 0.0f           ; m[3][3] = 1.0f;
    // return glm::transpose(m);
    return m;
} 
aiMatrix4x4 transByMat3AI(const aiMatrix3x3& AssimpMatrix)
{
    aiMatrix4x4 m;
    m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = 0.0f;
    m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = 0.0f;
    m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = 0.0f;
    m[3][0] = 0.0f           ; m[3][1] = 0.0f           ; m[3][2] = 0.0f           ; m[3][3] = 1.0f;
    // return glm::transpose(m);
    return m;
} 

glm::mat4 initTranslationTransform(float x, float y, float z)
{
    glm::mat4 m;
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    // return glm::transpose(m);
    return m;
}
aiMatrix4x4 initTranslationTransformAI(float x, float y, float z)
{
    aiMatrix4x4 m;
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    // return glm::transpose(m);
    return m;
}

glm::mat4 init()
{
    glm::mat4 m;
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    // return glm::transpose(m);;
    return m;
}

aiMatrix4x4 initAI()
{
    aiMatrix4x4 m;
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    // return glm::transpose(m);;
    return m;
}

void print(glm::mat4* m)
{
     std::cout << "mat4 start ------- " << std::endl;
    std::cout << "      " << (*m)[0][0] << " ";
    std::cout << (*m)[0][1] << " ";
    std::cout << (*m)[0][2] << " ";
    std::cout << (*m)[0][3] << " ";
    std::cout << std::endl;
    std::cout << "      " << (*m)[1][0] << " ";
    std::cout << (*m)[1][1] << " ";
    std::cout << (*m)[1][2] << " ";
    std::cout << (*m)[1][3] << " ";
    std::cout << std::endl;
    std::cout << "      " << (*m)[2][0] << " ";
    std::cout << (*m)[2][1] << " ";
    std::cout << (*m)[2][2] << " ";
    std::cout << (*m)[2][3] << " ";
    std::cout << std::endl;
    std::cout << "      " << (*m)[3][0] << " ";
    std::cout << (*m)[3][1] << " ";
    std::cout << (*m)[3][2] << " ";
    std::cout << (*m)[3][3] << " ";
    std::cout << std::endl << "mat4 end ------- " << std::endl;
}

void print_inline(glm::mat4* m)
{
    std::cout << "      " << (*m)[0][0] << " ";
    std::cout << (*m)[0][1] << " ";
    std::cout << (*m)[0][2] << " ";
    std::cout << (*m)[0][3] << " ";
    std::cout << "      " << (*m)[1][0] << " ";
    std::cout << (*m)[1][1] << " ";
    std::cout << (*m)[1][2] << " ";
    std::cout << (*m)[1][3] << " ";
    std::cout << "      " << (*m)[2][0] << " ";
    std::cout << (*m)[2][1] << " ";
    std::cout << (*m)[2][2] << " ";
    std::cout << (*m)[2][3] << " ";
    std::cout << "      " << (*m)[3][0] << " ";
    std::cout << (*m)[3][1] << " ";
    std::cout << (*m)[3][2] << " ";
    std::cout << (*m)[3][3] << " ";
    std::cout << std::endl;
}

void print(aiMatrix4x4* m)
{
     std::cout << "aiMatrix4x4 start ------- " << std::endl;
    std::cout << "      " << (*m)[0][0] << " ";
    std::cout << (*m)[0][1] << " ";
    std::cout << (*m)[0][2] << " ";
    std::cout << (*m)[0][3] << " ";
    std::cout << std::endl;
    std::cout << "      " << (*m)[1][0] << " ";
    std::cout << (*m)[1][1] << " ";
    std::cout << (*m)[1][2] << " ";
    std::cout << (*m)[1][3] << " ";
    std::cout << std::endl;
    std::cout << "      " << (*m)[2][0] << " ";
    std::cout << (*m)[2][1] << " ";
    std::cout << (*m)[2][2] << " ";
    std::cout << (*m)[2][3] << " ";
    std::cout << std::endl;
    std::cout << "      " << (*m)[3][0] << " ";
    std::cout << (*m)[3][1] << " ";
    std::cout << (*m)[3][2] << " ";
    std::cout << (*m)[3][3] << " ";
    std::cout << std::endl << "aiMatrix4x4 end ------- " << std::endl;
}

bool is_same_m4(glm::mat4* m1, glm::mat4* m2)
{
    return     (*m1)[0][0] == (*m2)[0][0] 
            && (*m1)[0][1] == (*m2)[0][1] 
            && (*m1)[0][2] == (*m2)[0][2] 
            && (*m1)[0][3] == (*m2)[0][3]

            && (*m1)[1][1] == (*m2)[1][1] 
            && (*m1)[1][2] == (*m2)[1][2] 
            && (*m1)[1][3] == (*m2)[1][3]
            && (*m1)[1][3] == (*m2)[1][3]

            && (*m1)[2][1] == (*m2)[2][1] 
            && (*m1)[2][2] == (*m2)[2][2] 
            && (*m1)[2][3] == (*m2)[2][3]
            && (*m1)[2][3] == (*m2)[2][3]

            && (*m1)[3][1] == (*m2)[3][1] 
            && (*m1)[3][2] == (*m2)[3][2] 
            && (*m1)[3][3] == (*m2)[3][3]
            && (*m1)[3][3] == (*m2)[3][3];
}

bool is_same_m4(glm::mat4* m1, aiMatrix4x4* m2)
{
    // m2->Transpose();
    return     (*m1)[0][0] == (*m2)[0][0] 
            && (*m1)[0][1] == (*m2)[0][1] 
            && (*m1)[0][2] == (*m2)[0][2] 
            && (*m1)[0][3] == (*m2)[0][3]

            && (*m1)[1][1] == (*m2)[1][1] 
            && (*m1)[1][2] == (*m2)[1][2] 
            && (*m1)[1][3] == (*m2)[1][3]
            && (*m1)[1][3] == (*m2)[1][3]

            && (*m1)[2][1] == (*m2)[2][1] 
            && (*m1)[2][2] == (*m2)[2][2] 
            && (*m1)[2][3] == (*m2)[2][3]
            && (*m1)[2][3] == (*m2)[2][3]

            && (*m1)[3][1] == (*m2)[3][1] 
            && (*m1)[3][2] == (*m2)[3][2] 
            && (*m1)[3][3] == (*m2)[3][3]
            && (*m1)[3][3] == (*m2)[3][3];
}

void copy(glm::mat4* m1, glm::mat4* m2)
{
    (*m1)[0][0] = (*m2)[0][0];  (*m1)[0][1] = (*m2)[0][1];  (*m1)[0][2] = (*m2)[0][2];  (*m1)[0][3] = (*m2)[0][3];
    (*m1)[1][0] = (*m2)[1][0];  (*m1)[1][1] = (*m2)[1][1];  (*m1)[1][2] = (*m2)[1][2];  (*m1)[1][3] = (*m2)[1][3];
    (*m1)[2][0] = (*m2)[2][0];  (*m1)[2][1] = (*m2)[2][1];  (*m1)[2][2] = (*m2)[2][2];  (*m1)[2][3] = (*m2)[2][3];
    (*m1)[3][0] = (*m2)[3][0];  (*m1)[3][1] = (*m2)[3][1];  (*m1)[3][2] = (*m2)[3][2];  (*m1)[3][3] = (*m2)[3][3];
}

#endif