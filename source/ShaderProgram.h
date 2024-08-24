#ifndef RAYTRACING_SHADERPROGRAM_H
#define RAYTRACING_SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
typedef unsigned int uint;

class ShaderProgram {
public:
    uint id;

    ShaderProgram(const std::vector< std::pair< std::string,uint > > &shaders);
    ~ShaderProgram();
    void use() const;
    void setVec3(const std::string& name, const glm::vec3& vec);
    void setIVec3(const std::string& name, const glm::ivec3& vec);
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);

private:

    uint attachShader(const std::string & path, uint type) const;
    static void checkCompileErrors(uint shader, const std::string & typeName);
    static void checkLinkErrors(uint program);

};


#endif
