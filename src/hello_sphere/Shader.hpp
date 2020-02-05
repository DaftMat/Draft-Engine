/**
 * Created by mathis on 01/02/2020.
 */

#ifndef DAFT_ENGINE_SHADER_HPP
#define DAFT_ENGINE_SHADER_HPP

#include <opengl_stuff.h>

class Shader {
public:
    Shader(const char * vertexPath, const char * fragmentPath);

    ~Shader();

    /**
     * Activates the shaders
     */
    void use() const;

    ///Uniform utils
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 & value) const;
    void setVec3(const std::string &name, const glm::vec3 & value) const;
    void setVec4(const std::string &name, const glm::vec4 & value) const;
    void setMat2(const std::string &name, const glm::mat2 & value) const;
    void setMat3(const std::string &name, const glm::mat3 & value) const;
    void setMat4(const std::string &name, const glm::mat4 & value) const;

private:
    void setupShader(const char * vertexCode, const char * fragmentCode);
    void checkCompileError(GLuint shader, std::string type) const;

    GLuint m_ID;
};


#endif //DAFT_ENGINE_SHADER_HPP
