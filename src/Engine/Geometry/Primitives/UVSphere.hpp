//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_SPHERE_HPP
#define DAFT_ENGINE_SPHERE_HPP

#include "src/Engine/Geometry/Model.hpp"

/** A Model that draws a sphere from UV. */
class UVSphere : public Model
{
  public:
    /** Constructor.
     *
     * @param meridians - number of meridians on the uv sphere.
     * @param parallels - number of parallels on the uv sphere.
     */
    explicit UVSphere( GLuint meridians = 32, GLuint parallels = 16 );

    /** reset the uv sphere.
     * delete and create uv sphere AOs and BOs in OpenGL context.
     */
    void reset() override { setupUvSphere(); }

    /** uv sphere's type getter.
     * c.f. ModelType
     * @return UV_SPHERE
     */
    ModelType getType() const override { return UV_SPHERE; }

    /** uv sphere's string type getter.
     *
     * @return "UV Sphere"
     */
    std::string getTypeAsString() const override { return "UV Sphere"; }

    /** uv sphere's settings setter.
     * c.f. ModelParam
     * @param params - new settings of uv sphere
     */
    void editModel( const ModelParam& params ) override;

    /** uv sphere's settings getter.
     * c.f. ModelParam
     * @return settings of uv sphere
     */
    ModelParam getParams() const override;

  private:
    void setupUvSphere();
    void setMeridians( GLuint meridians ) { m_sectors = glm::max<GLuint>( meridians, 4 ); }
    void setParallels( GLuint parallels ) { m_stacks = glm::max<GLuint>( parallels, 4 ); }
    GLuint getMeridians() const { return m_sectors; }
    GLuint getParallels() const { return m_stacks; }

    GLuint m_stacks;
    GLuint m_sectors;
};

#endif // DAFT_ENGINE_SPHERE_HPP
