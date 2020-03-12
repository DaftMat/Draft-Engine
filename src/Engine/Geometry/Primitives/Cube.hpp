//
// Created by mathis on 05/03/2020.
//

#ifndef DAFT_ENGINE_CUBE_HPP
#define DAFT_ENGINE_CUBE_HPP

#include <src/Engine/Geometry/Model.hpp>

/** Cube class.
 *  A Model that draws a cube.
 */
class Cube : public Model
{
  public:
    /** Constructor.
     *
     * @param resolution - resolution of the cube's faces
     */
    explicit Cube( GLuint resolution = 2 ) : m_resolution{resolution} { setupCube(); }

    void reset() override { setupCube(); }

    /** cube's type getter. */
    ModelType getType() const override { return CUBE; }
    /** cube's string type getter. */
    std::string getTypeAsString() const override { return "Cube"; }
    /** cube's settings setter */
    void editModel( const ModelParam& params ) override;
    /** cube's settings getter */
    ModelParam getParams() const override;

  private:
    void setupCube();

    GLuint m_resolution;
};

#endif // DAFT_ENGINE_CUBE_HPP
