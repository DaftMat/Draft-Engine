//
// Created by mathis on 11/02/2020.
//

#ifndef DAFT_ENGINE_ICOSPHERE_HPP
#define DAFT_ENGINE_ICOSPHERE_HPP

#include <src/Engine/Geometry/Model.hpp>

/** A Model that draws a sphere from a subdivided Icosahedron. */
class IcoSphere : public Model
{
  public:
    /** Constructor.
     *
     * @param subs - number of subdivisions to apply.
     */
    explicit IcoSphere( GLuint subs = 3 );

    /** reset the ico sphere.
     * delete and create ico sphere AOs and BOs in OpenGL context.
     */
    void reset() override { setupIcoSphere(); }

    /** ico sphere's type getter.
     * c.f. ModelType
     * @return ICO_SPHERE
     */
    ModelType getType() const override { return ICO_SPHERE; }

    /** ico sphere's string type getter.
     *
     * @return "Ico Sphere"
     */
    std::string getTypeAsString() const override { return "Ico Sphere"; }

    /** ico sphere's settings setter.
     * c.f. ModelParam
     * @param params - new settings of the ico sphere.
     */
    void editModel( const ModelParam& params ) override;

    /** ico sphere's settings getter.
     * c.f. ModelParam
     * @return settings of the ico sphere.
     */
    ModelParam getParams() const override;

  private:
    void setupIcoSphere();
    void setSubdivisions( int subs ) { m_subs = subs >= 0 ? subs : 0; }
    void setupIcosahedron();
    void setupSubdivisions();
    static glm::vec3 computeHalfVertex( const glm::vec3& a, glm::vec3& b ) {
        return glm::normalize( a + b );
    }

    GLuint m_subs;
};
/// Utils functions
GLuint mod1( GLuint x, GLuint m );

#endif // DAFT_ENGINE_ICOSPHERE_HPP
