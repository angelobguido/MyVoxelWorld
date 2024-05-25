#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

ShaderProgram::ShaderProgram(const std::vector<std::pair<std::string, uint>> &shaders) {

    id = glCreateProgram();

    std::vector<uint> shaderIDs;

    for (auto & shader : shaders){
        uint shaderID = attachShader(shader.first, shader.second);
        shaderIDs.push_back(shaderID);
        glAttachShader(id, shaderID);
    }

    glLinkProgram(id);
    checkLinkErrors(id);

    // Detach and delete shaders after linking
    for (uint shaderID : shaderIDs) {
        glDetachShader(id, shaderID);
        glDeleteShader(shaderID);
    }
}

uint ShaderProgram::attachShader(const std::string &path, uint type) const {

    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << path << std::endl;
        return 0;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    const char* cString = content.c_str();

    file.close();

    uint shader = glCreateShader(type);
    glShaderSource(shader, 1, &cString, nullptr);
    glCompileShader(shader);

    std::string typeName;

    switch (type) {
        case GL_VERTEX_SHADER:
            typeName = "Vertex";
            break;

        case GL_FRAGMENT_SHADER:
            typeName = "Fragment";
            break;

        case GL_COMPUTE_SHADER:
            typeName = "Compute";
            break;

        default:
            typeName = "Unknown";
            break;
    }

    checkCompileErrors(shader, typeName);

    return shader;
}

void ShaderProgram::checkCompileErrors(uint shader, const std::string &typeName) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int infoLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
        std::vector<char> info(infoLength);
        glGetShaderInfoLog(shader, infoLength, NULL, info.data());
        std::cerr << "ERROR: " << typeName << " Shader Compilation Failed\n" << info.data() << std::endl;
    }
}

void ShaderProgram::checkLinkErrors(uint program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int infoLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        std::vector<char> info(infoLength);
        glGetProgramInfoLog(program, infoLength, NULL, info.data());
        std::cerr << "ERROR: Program Linking Failed\n" << info.data() << std::endl;
    }
}

void ShaderProgram::use() const {
    glUseProgram(id);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& vec) {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vec));
}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, 0, glm::value_ptr(mat));
}

void ShaderProgram::setIVec3(const std::string &name, const glm::ivec3 &vec) {
    glUniform3iv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vec));
}
