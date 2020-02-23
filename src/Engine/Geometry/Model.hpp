//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_MODEL_HPP
#define DAFT_ENGINE_MODEL_HPP


#include <memory>
#include "Mesh.hpp"
#include "src/Engine/Shader.hpp"

union ModelParam {
    struct uvs_param {
        GLuint meridians;
        GLuint parallels;
    }uv_sphere;
    struct icos_param {
        GLuint subdivisions;
    }ico_sphere;
    struct cubs_param {
        GLuint resolution;
    }cube_sphere;
};

enum ModelType {
    MODEL,
    UV_SPHERE,
    ICO_SPHERE,
    CUBE_SPHERE
};

class Model {
public:
    Model() = default;
    virtual ~Model() { m_meshes.clear(); }

    Model(const Model &) = delete;
    Model & operator=(const Model &) = delete;

    void draw(const Shader &shader) const;

    glm::mat4 model() const { return m_translateMat * rotation() * scale(); }

    void translate(const glm::vec3 & t);

    void setPosition(const glm::vec3 &new_pos);
    const glm::vec3 & getPosition() const { return m_position; }
    void setRotation(const glm::vec3 &new_rot);
    const glm::vec3 & getRotation() const { return m_rotation; }
    void setScale(const glm::vec3 &new_scale);
    const glm::vec3 & getScale() const { return m_scale; }

    virtual void reset() {}

    virtual ModelType getType() const { return MODEL; }
    virtual std::string getTypeAsString() const { return "Object"; }
    virtual void editModel(const ModelParam &params) = 0;
    virtual ModelParam getParams() const = 0;

protected:
    std::vector<std::unique_ptr<Mesh>> m_meshes;

private:
    glm::mat4 rotation() const;
    glm::mat4 scale() const { return glm::scale(glm::mat4(), m_scale); }

    glm::vec3 m_position {0.f, 0.f, 0.f};
    glm::vec3 m_rotation {0.f, 0.f, 0.f};
    glm::vec3 m_scale {1.f, 1.f, 1.f};

    glm::mat4 m_translateMat;
};


#endif //DAFT_ENGINE_MODEL_HPP
