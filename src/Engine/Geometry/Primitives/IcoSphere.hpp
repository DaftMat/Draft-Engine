//
// Created by mathis on 11/02/2020.
//

#ifndef DAFT_ENGINE_ICOSPHERE_HPP
#define DAFT_ENGINE_ICOSPHERE_HPP

#include <src/Engine/Geometry/Model.hpp>

/** IcoSphere Class.
 *  A Model that draws a sphere from a subdivided Icosahedron.
 */
class IcoSphere : public Model
{
  public:
    /** Constructor.
     *
     * @param subs - number of subdivisions to apply.
     */
    explicit IcoSphere( GLuint subs = 3 );

    void reset() override { setupIcoSphere(); }

    /** Adds a subdivision layer. **/
    void subdivide() { ++m_subs; }
    /** Removes a subdivision layer. **/
    void unsubdivide() {
        if ( m_subs > 0 ) --m_subs;
    }

    /** Subdivision number getter. */
    GLuint getSubdivisions() const { return m_subs; }
    /** Subdivision number setter. */
    void setSubdivisions( int subs ) { m_subs = subs >= 0 ? subs : 0; }

    /** ico sphere's type getter. */
    ModelType getType() const override { return ICO_SPHERE; }
    /** ico sphere's string type getter. */
    std::string getTypeAsString() const override { return "Ico Sphere"; }
    /** ico sphere's settings setter. */
    void editModel( const ModelParam& params ) override;
    /** ico sphere's settings getter. */
    ModelParam getParams() const override;

  private:
    void setupIcoSphere() {
        setupIcosahedron();
        for ( GLuint i = 0; i < m_subs; ++i )
            setupSubdivisions();
    }
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
