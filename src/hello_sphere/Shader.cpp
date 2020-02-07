/**
 * Created by mathis on 01/02/2020.
 */

#include <fstream>
#include <sstream>
#include "Shader.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        /// Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        /// close files
        vShaderFile.close();
        fShaderFile.close();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR:SHADER:FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    /// Vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileError(vertex, "VERTEX");
    /// Fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileError(fragment, "FRAGMENT");
    /// Program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    checkLinkError(m_ID);
    /// Release shader
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(m_ID);
}

void Shader::use() const {
    glUseProgram(m_ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (GLuint)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 & value) const {
    glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, const glm::vec3 & value) const {
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, const glm::vec4 & value) const {
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat2(const std::string &name, const glm::mat2 & value) const {
    glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string &name, const glm::mat3 & value) const {
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, const glm::mat4 & value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileError(GLuint shader, const std::string & type) const {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR:SHADER_COMPILATION of type : " << type << "\n" << infoLog << std::endl;
    }
}

void Shader::checkLinkError(GLuint program) const {
    int success;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cout << "ERROR:PROGRAM_LINKING\n" << infoLog << std::endl;
    }
}