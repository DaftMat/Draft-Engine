//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_SPHERE_HPP
#define DAFT_ENGINE_SPHERE_HPP

#include "Model.hpp"

/**
 * Mother class of sphere model object
 */
class UVSphere : public Model {
public:
    UVSphere() : m_stacks { 32 }, m_sectors { 64 } { setupUvSphere(); }
    UVSphere(GLuint meridians, GLuint parallels) : m_stacks { meridians }, m_sectors { parallels } { setupUvSphere(); }

    void setMeridians(GLuint meridians) { m_stacks = meridians; }
    void setParallels(GLuint parallels) { m_sectors = parallels; }

    GLuint getMeridians() { return m_stacks; }
    GLuint getParallels() { return m_sectors; }

    void reset() override { setupUvSphere(); }

private:
    void setupUvSphere() override;

    GLuint m_stacks;
    GLuint m_sectors;
};


#endif //DAFT_ENGINE_SPHERE_HPP
