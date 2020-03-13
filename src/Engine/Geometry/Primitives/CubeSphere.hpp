//
// Created by mathis on 13/02/2020.
//

#ifndef DAFT_ENGINE_CUBESPHERE_HPP
#define DAFT_ENGINE_CUBESPHERE_HPP

#include <src/Engine/Geometry/Model.hpp>

/** A Model that draws a sphere from a cube. */
class CubeSphere : public Model
{
  public:
    /** Constructor.
     *
     * @param resolution - resolution of the cube sphere's faces.
     */
    explicit CubeSphere( GLuint resolution = 16 );

    /** reset the cube sphere.
     * delete and create cube sphere AOs and BOs in OpenGL context.
     */
    void reset() override { setupCubeSphere(); }

    /** cube sphere's type getter.
     * c.f. ModelType
     * @return CUBE_SPHERE
     */
    ModelType getType() const override { return CUBE_SPHERE; }

    /** cube sphere's string type getter.
     *
     * @return "Cube Sphere"
     */
    std::string getTypeAsString() const override { return "Cube Sphere"; }

    /** cube sphere's settings setter.
     * c.f. ModelParam
     * @param params - new settings of the cube sphere.
     */
    void editModel( const ModelParam& params ) override;

    /** cube sphere's settings getter.
     * c.f. ModelParam
     * @return settings of the cube sphere.
     */
    ModelParam getParams() const override;

  private:
    void setupCubeSphere();

    GLuint m_resolution;
};

#endif // DAFT_ENGINE_CUBESPHERE_HPP
