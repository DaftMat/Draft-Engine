//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_SPHERE_HPP
#define DAFT_ENGINE_SPHERE_HPP

#include "src/Engine/Geometry/Model.hpp"

/**
 * Mother class of sphere model object
 */
class UVSphere : public Model {
public:
    explicit UVSphere(GLuint meridians = 8, GLuint parallels = 8) : m_stacks { parallels }, m_sectors { meridians }
    { setupUvSphere(); }

    void setMeridians(GLuint meridians) { m_sectors = glm::max<GLuint>(meridians, 4); }
    void setParallels(GLuint parallels) { m_stacks = glm::max<GLuint>(parallels, 4); }

    GLuint getMeridians() const { return m_sectors; }
    GLuint getParallels() const { return m_stacks; }

    void reset() override { setupUvSphere(); }

    ModelType getType() const override { return UV_SPHERE; }
    std::string getTypeAsString() const override { return "UV Sphere"; }
    void editModel(const ModelParam &params) override;
    ModelParam getParams() const override;

private:
    void setupUvSphere();

    GLuint m_stacks;
    GLuint m_sectors;
};


#endif //DAFT_ENGINE_SPHERE_HPP