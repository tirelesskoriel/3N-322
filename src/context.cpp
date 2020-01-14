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