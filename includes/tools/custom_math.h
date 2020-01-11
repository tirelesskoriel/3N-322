
#ifndef CUSTOM_MATH
#define CUSTOM_MATH

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <iostream>

glm::mat4 transByMat4(const aiMatrix4x4* aim4);

void transByMat4(glm::mat4* mat4, const aiMatrix4x4* aim4);

glm::mat4 initScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
aiMatrix4x4 initScaleTransformAI(float ScaleX, float ScaleY, float ScaleZ);

glm::mat4 transByMat3(const aiMatrix3x3& AssimpMatrix);
aiMatrix4x4 transByMat3AI(const aiMatrix3x3& AssimpMatrix);

glm::mat4 initTranslationTransform(float x, float y, float z);
aiMatrix4x4 initTranslationTransformAI(float x, float y, float z);

glm::mat4 init();

aiMatrix4x4 initAI();

void print(glm::mat4* m);

void print_inline(glm::mat4* m);

void print(aiMatrix4x4* m);

bool is_same_m4(glm::mat4* m1, glm::mat4* m2);

bool is_same_m4(glm::mat4* m1, aiMatrix4x4* m2);

void copy(glm::mat4* m1, glm::mat4* m2);

#endif