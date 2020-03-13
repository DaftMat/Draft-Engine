//
// Created by mathis on 01/02/2020.
//

#ifndef DAFT_ENGINE_SHADER_HPP
#define DAFT_ENGINE_SHADER_HPP

#include <Engine/Geometry/Lights/Light.hpp>
#include <opengl_stuff.h>
#include <src/Engine/Geometry/Material.hpp>

/** Shader Class.
 *  Regroups a vertex and a fragment shader in a program shader.
 */
class Shader
{
  public:
    /** Constructor.
     *
     * @param vertexPath - vertex shader's GLSL file path
     * @param fragmentPath - fragment shader's GLSL file path
     */
    Shader( const char* vertexPath, const char* fragmentPath ) :
        m_vertexpath{vertexPath},
        m_fragmentpath{fragmentPath} {
        reset();
    }

    ~Shader();

    /** Activates the shaders. */
    void use() const;

    /** Boolean uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - boolean value to assign.
     */
    void setBool( const std::string& name, bool value ) const;

    /** Integer uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - integer value to assign.
     */
    void setInt( const std::string& name, int value ) const;

    /** Float uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - float value to assign.
     */
    void setFloat( const std::string& name, float value ) const;

    /** Vector2 uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - vector2 value to assign.
     */
    void setVec2( const std::string& name, const glm::vec2& value ) const;

    /** Vector3 uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - vector3 value to assign.
     */
    void setVec3( const std::string& name, const glm::vec3& value ) const;

    /** Vector4 uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - vector4 to assign.
     */
    void setVec4( const std::string& name, const glm::vec4& value ) const;

    /** Matrix2 uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - matrix2 to assign.
     */
    void setMat2( const std::string& name, const glm::mat2& value ) const;

    /** Matrix3 uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - matrix3 to assign.
     */
    void setMat3( const std::string& name, const glm::mat3& value ) const;

    /** Matrix4 uniform setter.
     *
     * @param name - name of the uniform variable.
     * @param value - matrix4 to assign.
     */
    void setMat4( const std::string& name, const glm::mat4& value ) const;

    /** Material uniform setter.
     *
     * @param material - material to assign.
     */
    void setMaterial( const Material& material ) const;

    /** Adds a Light to the shader's lights.
     *
     * @param light - light to add.
     */
    void addLight( Light* light );

    /** Clears all lights from the shader. */
    void clearLights();

    void reset();

  private:
    void checkCompileError( GLuint shader, const std::string& type ) const;
    void checkLinkError( GLuint program ) const;

    void setBaseLight( Light* light, const std::string& lightType, GLuint index );

    GLuint m_ID;
    GLuint m_num_point_light{0};
    GLuint m_num_dir_light{0};
    GLuint m_num_spot_light{0};

    const char* m_vertexpath;
    const char* m_fragmentpath;
};

#endif // DAFT_ENGINE_SHADER_HPP
