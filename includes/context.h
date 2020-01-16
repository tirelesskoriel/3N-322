#ifndef CONTEXT
#define CONTEXT

#include <model/surround_camera.h>
#include <tools/filesystem.h>
#include <string>

class Context
{
public:
    SurroundCamera* camera;
    size_t scrWidth;
    size_t scrHeight;
    FileSystem* fileSystem;
    Context(SurroundCamera* camera, size_t scrWidth, size_t scrHeight);
    ~Context();
    std::string getPath(const std::string& path);
    float W2H();
    float H2W();
    float W2W();
    float H2H();
};

#endif