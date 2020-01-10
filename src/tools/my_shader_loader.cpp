#include "tools/my_shader_loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

ShaderLoader::ShaderLoader(const GLchar* vsp, const GLchar* fsp)
{
    std::string vsc, fsc;
    std::ifstream vsf, fsf;

    vsf.exceptions(std::ios::badbit | std::ios::failbit);
    fsf.exceptions(std::ios::badbit | std::ios::failbit);

    try
    {
        vsf.open(vsp);
        fsf.open(fsp);

        std::stringstream vss, fss;

        vss << vsf.rdbuf();
        fss << fsf.rdbuf();

        vsc = vss.str();
        fsc = fss.str();

        vsf.close();
        fsf.close();
    }
    catch(std::ifstream::failure e)
    {
        vsf.close();
        fsf.close();
        std::cerr << "Failed read shader!!" << std::endl;
    }

    const GLchar* vertexShaderSource = vsc.c_str();
    const GLchar* fragShaderSource = fsc.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fShader, 1, &fragShaderSource, NULL);

    glCompileShader(vShader);
    check_complie(vShader, "VS");
    glCompileShader(fShader);
    check_complie(fShader, "FS");
    
    ID = glCreateProgram();
    glAttachShader(ID, vShader);
    glAttachShader(ID, fShader);
    glLinkProgram(ID);
    check_complie(ID, "PROGRAM");

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    
};

void ShaderLoader::check_complie(GLuint id, const std::string type)
{
    GLint success;
    GLchar infoLog[512];

    if (type == "PROGRAM") 
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINK_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else 
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void ShaderLoader::use() 
{ 
    glUseProgram(ID); 
}

void ShaderLoader::unuse() 
{ 
    glDeleteProgram(ID); 
}
void ShaderLoader::setBool(const std::string &name, GLboolean value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (GLint)value); 
}
void ShaderLoader::setInt(const std::string &name, GLint value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void ShaderLoader::setFloat(const std::string &name, GLfloat value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}
void ShaderLoader::setVec2(const std::string &name, const glm::vec2 &value) const
{ 
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void ShaderLoader::setVec2(const std::string &name, GLfloat x, GLfloat y) const
{ 
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
}
void ShaderLoader::setVec3(const std::string &name, const glm::vec3 &value) const
{ 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void ShaderLoader::setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const
{ 
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
}
void ShaderLoader::setVec4(const std::string &name, const glm::vec4 &value) const
{ 
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void ShaderLoader::setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{ 
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
}
void ShaderLoader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderLoader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderLoader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderLoader::setMat4Ordering(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, &mat[0][0]);
}

