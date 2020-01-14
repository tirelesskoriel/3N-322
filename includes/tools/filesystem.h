#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <cstdlib>

class FileSystem
{
private:
  std::string getPathRelativeRoot(const std::string& root, const std::string& path);
  std::string getPathRelativeBinary(const std::string& path);
public:
  std::string getPath(const std::string& path);
};

#endif
