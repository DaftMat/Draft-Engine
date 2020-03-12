//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_SPHERE_HPP
#define DAFT_ENGINE_SPHERE_HPP

#include "src/Engine/Geometry/Model.hpp"

/** UVSphere Class.
 *  A Model that draws a sphere from UV.
 */
class UVSphere : public Model
{
  public:
    /** Constructor.
     *
     * @param meridians - number of meridians on the uv sphere.
     * @param parallels - number of parallels on the uv sphere.
     */
    explicit UVSphere( GLuint meridians = 32, GLuint parallels = 16 );

    /** Meridians setter. */
    void setMeridians( GLuint meridians ) { m_sectors = glm::max<GLuint>( meridians, 4 ); }
    /** Parallels setter. */
    void setParallels( GLuint parallels ) { m_stacks = glm::max<GLuint>( parallels, 4 ); }

    /** Meridians getter. */
    GLuint getMeridians() const { return m_sectors; }
    /** Meridians setter. */
    GLuint getParallels() const { return m_stacks; }

    void reset() override { setupUvSphere(); }

    /** uv sphere's type getter */
    ModelType getType() const override { return UV_SPHERE; }
    /** uv sphere's string type getter */
    std::string getTypeAsString() const override { return "UV Sphere"; }
    /** uv sphere's settings setter */
    void editModel( const ModelParam& params ) override;
    /** uv sphere's settings getter */
    ModelParam getParams() const override;

  private:
    void setupUvSphere();

    GLuint m_stacks;
    GLuint m_sectors;
};

#endif // DAFT_ENGINE_SPHERE_HPP
