#ifndef TEXTURE_2D
#define TEXTURE_2D

#include <glad/glad.h>

class Texture2d{
public:
    GLuint ID;
    Texture2d(const char* path);
    void active(GLint tp);
};

#endif