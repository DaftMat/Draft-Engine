//
// Created by mathis on 13/02/2020.
//

#ifndef DAFT_ENGINE_CUBESPHERE_HPP
#define DAFT_ENGINE_CUBESPHERE_HPP

#include <src/Engine/Geometry/Model.hpp>

/** CubeSphere Class.
 *  A Model that draws a sphere from a cube.
 */
class CubeSphere : public Model
{
  public:
    /** Constructor.
     *
     * @param resolution - resolution of the cube sphere's faces.
     */
    explicit CubeSphere( GLuint resolution = 16 );

    void reset() override { setupCubeSphere(); }

    /** cube sphere's type getter. */
    ModelType getType() const override { return CUBE_SPHERE; }
    /** cube sphere's string type getter. */
    std::string getTypeAsString() const override { return "Cube Sphere"; }
    /** cube sphere's settings setter. */
    void editModel( const ModelParam& params ) override;
    /** cube sphere's settings getter. */
    ModelParam getParams() const override;

  private:
    void setupCubeSphere();

    GLuint m_resolution;
};

#endif // DAFT_ENGINE_CUBESPHERE_HPP
