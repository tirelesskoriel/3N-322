#ifndef OPENGL_RENNDER
#define OPENGL_RENNDER

#include <model/model.h>
#include <context.h>

const unsigned int FRAME_BUFFER_COUNT = 8;

class OpenglRender
{
private:
    Model* model;
    Context* context;
    ShaderLoader* seShader;
    const ShaderLoader* modelShader;
    
    unsigned int textureVAO = 0;

    unsigned int framebuffers[FRAME_BUFFER_COUNT] = {0};
    unsigned int textureColorbuffers[FRAME_BUFFER_COUNT] = {0};
    

public:
    bool is_3n = false;
    unsigned int PL = 1.0f;

    OpenglRender(Context* context);
    ~OpenglRender();
    void render();
};
#endif