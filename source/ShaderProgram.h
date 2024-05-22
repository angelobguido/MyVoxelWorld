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

class ShaderProgram {
public:
    uint id;

    ShaderProgram(const std::vector< std::pair< std::string,uint > > &shaders);
    ~ShaderProgram();
    void use() const;
    void setVec3(const std::string& name, const glm::vec3& vec);

private:

    uint attachShader(const std::string & path, uint type) const;
    static void checkCompileErrors(uint shader, const std::string & typeName);
    static void checkLinkErrors(uint program);

};


#endif
