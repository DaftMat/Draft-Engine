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
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileError(vertex, "VERTEX");
    /// Fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
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

    // make sure the lights lists are cleared
    use();
    clearLights();
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

void Shader::addLight(Light * light) {
    std::string lightType;
    LightParam params = light->getParams();
    switch (light->getType()) {
        case POINT_LIGHT: {
            lightType = "point_light";
            setBaseLight(light, lightType, m_num_point_light);
            setVec3(lightType + "[" + std::to_string(m_num_point_light) + "].position", params.pointlight.position);
            setFloat(lightType + "[" + std::to_string(m_num_point_light) + "].constant", params.pointlight.constant);
            setFloat(lightType + "[" + std::to_string(m_num_point_light) + "].linear", params.pointlight.linear);
            setFloat(lightType + "[" + std::to_string(m_num_point_light) + "].quadratic", params.pointlight.quadratic);
            setInt("num_point_light", ++m_num_point_light);
            break;
        } case DIR_LIGHT:
            lightType = "dir_light";
            setBaseLight(light, lightType, m_num_dir_light);
            setVec3(lightType + "[" + std::to_string(m_num_dir_light) + "].direction", params.dirlight.direction);
            setInt("num_dir_light", ++m_num_dir_light);
            break;
        default:
            return;
    }
}

void Shader::setBaseLight(Light *light, const std::string & lightType, GLuint index) {
    setVec3(lightType + "[" + std::to_string(index) +"].ambient", light->ambient());
    setVec3(lightType + "[" + std::to_string(index) +"].diffuse", light->diffuse());
    setVec3(lightType + "[" + std::to_string(index) +"].specular", light->specular());
}

void Shader::clearLights() {
    m_num_point_light = 0;
    m_num_dir_light = 0;
    m_num_spot_light = 0;
    setInt("num_point_light", 0);
    setInt("num_dir_light", 0);
    setInt("num_spot_light", 0);
}

void Shader::checkCompileError(GLuint shader, const std::string & type) const {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "ERROR:SHADER_COMPILATION of type : " << type << "\n" << infoLog << std::endl;
    }
}

void Shader::checkLinkError(GLuint program) const {
    int success;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cerr << "ERROR:PROGRAM_LINKING\n" << infoLog << std::endl;
    }
}
