/**
 * Created by mathis on 01/02/2020.
 */

#ifndef DAFT_ENGINE_SHADER_HPP
#define DAFT_ENGINE_SHADER_HPP

#include <opengl_stuff.h>

class Shader {
public:
    Shader(const char * vertexPath, const char * fragmentPath);

    Shader(const Shader & shader) { m_ID = shader.m_ID; }
    Shader(Shader && shader) noexcept { m_ID = shader.m_ID; }

    Shader & operator=(const Shader & shader) { m_ID = shader.m_ID; return *this; }
    Shader & operator=(Shader && shader) { m_ID = shader.m_ID; return *this; }

    ~Shader();

    /**
     * Activates the shaders
     */
    void use() const;

    ///Uniform utils
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, glm::vec2 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec4(const std::string &name, glm::vec4 value) const;
    void setMat2(const std::string &name, glm::mat2 value) const;
    void setMat3(const std::string &name, glm::mat3 value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

private:
    void setupShader(const char * vertexCode, const char * fragmentCode);
    void checkCompileError(GLuint shader, std::string type) const;

    GLuint m_ID;
};


#endif //DAFT_ENGINE_SHADER_HPP
