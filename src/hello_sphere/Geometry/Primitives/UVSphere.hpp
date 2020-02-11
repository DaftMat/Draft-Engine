//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_SPHERE_HPP
#define DAFT_ENGINE_SPHERE_HPP

#include "src/hello_sphere/Geometry/Model.hpp"

/**
 * Mother class of sphere model object
 */
class UVSphere : public Model {
public:
    explicit UVSphere(GLuint meridians = 8, GLuint parallels = 8) : m_stacks { meridians }, m_sectors { parallels } { setupUvSphere(); }

    void setMeridians(GLuint meridians) { m_stacks = meridians > 3 ? meridians : 4; }
    void setParallels(GLuint parallels) { m_sectors = parallels > 3 ? parallels : 4; }

    GLuint getMeridians() { return m_stacks; }
    GLuint getParallels() { return m_sectors; }

    void reset() override { setupUvSphere(); }

private:
    void setupUvSphere() override;

    GLuint m_stacks;
    GLuint m_sectors;
};


#endif //DAFT_ENGINE_SPHERE_HPP
