#include<context.h>

Context::Context(SurroundCamera* camera, size_t scr_width, size_t scr_height)
    :camera(camera), scrWidth(scr_width), scrHeight(scr_height)
{
    fileSystem = new FileSystem();
}

Context::~Context()
{
    delete fileSystem;
}

std::string Context::getPath(const std::string& path)
{
    return fileSystem->getPath(path);
}

float Context::W2H()
{
    return scrWidth / (float)scrHeight;
}
float Context::Context::H2W()
{
    return scrHeight / (float)scrWidth;
}
float Context::W2W()
{
    return scrWidth / (float)scrWidth;
}
float Context::Context::H2H()
{
    return scrHeight / (float)scrHeight;
}