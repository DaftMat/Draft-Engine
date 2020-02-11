/**
 * Created by mathis on 11/02/2020.
 */

#ifndef DAFT_ENGINE_ICOSPHERE_HPP
#define DAFT_ENGINE_ICOSPHERE_HPP


#include <src/hello_sphere/Geometry/Model.hpp>

class IcoSphere : public Model {
public:
    explicit IcoSphere(GLuint subs = 0) : m_subs { subs } { setupIcoSphere(); }

    void reset() override { setupIcoSphere(); }

    void subdivide() { ++m_subs; setupIcoSphere(); }
    void unsubdivide() { if (m_subs > 0) --m_subs; setupIcosahedron(); }

private:
    void setupIcoSphere() override { setupIcosahedron(); for (GLuint i = 0 ; i < m_subs ; ++i) setupSubdivisions(); }
    void setupIcosahedron();
    void setupSubdivisions();
    static glm::vec3 computeHalfVertex(const glm::vec3 &a, glm::vec3 &b) { return glm::normalize(a + b); }

    GLuint m_subs;
};
/// Utils functions
GLuint mod1(GLuint x, GLuint m);

#endif //DAFT_ENGINE_ICOSPHERE_HPP
