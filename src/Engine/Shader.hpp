/**
 * Created by mathis on 01/02/2020.
 */

#ifndef DAFT_ENGINE_SHADER_HPP
#define DAFT_ENGINE_SHADER_HPP

#include <opengl_stuff.h>
#include <Engine/Geometry/Lights/Light.hpp>

enum ShaderSelection {
    DEFAULT = 0,
    ERROR,
    BLINNPHONG,
    PBR
};

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

    void addLight(Light * light);
    void clearLights();

private:
    void checkCompileError(GLuint shader, const std::string & type) const;
    void checkLinkError(GLuint program) const;

    void setBaseLight(Light * light, const std::string & lightType, GLuint index);

    GLuint m_ID;
    GLuint m_num_point_light { 0 };
    GLuint m_num_dir_light { 0 };
    GLuint m_num_spot_light { 0 };
};


#endif //DAFT_ENGINE_SHADER_HPP
