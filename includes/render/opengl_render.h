#ifndef OPENGL_RENNDER
#define OPENGL_RENNDER

#include <model/model.h>
#include <context.h>

class OpenglRender
{
private:
    Model* model;
    Context* context;
public:
    OpenglRender(Context* context);
    ~OpenglRender();
    void render();
};
#endif