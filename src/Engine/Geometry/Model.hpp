//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_MODEL_HPP
#define DAFT_ENGINE_MODEL_HPP


#include <memory>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <src/Utils/adapters.hpp>

#include "Mesh.hpp"
#include "Material.hpp"
#include <Engine/Physics/Obb.hpp>
#include <Utils/types.hpp>

class Shader;

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
    struct cube_param {
        GLuint resolution;
    }cube;
    struct cylinder_param {
        GLuint meridians;
        GLuint parallels;
    }cylinder;
    struct cone_param {
        GLuint meridians;
        GLuint parallels;
    }cone;
    struct pyramid_param {
        GLuint resolution;
        GLuint parallels;
    }pyramid;
    struct tore_param {
        GLuint meridians;
        GLuint parallels;
    }tore;
};

enum ModelType {
    MODEL,
    UV_SPHERE,
    ICO_SPHERE,
    CUBE_SPHERE,
    CUBE,
    CYLINDER,
    CONE,
    PYRAMID,
    TORE
};

class Model {
public:
    Model() = default;
    Model(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, bool wire = false);
    virtual ~Model() { m_meshes.clear(); }

    Model(const Model &) = delete;
    Model & operator=(const Model &) = delete;

    void draw(const Shader &shader) const;

    glm::mat4 model() const { return m_translateMat * m_rotateMat * scale(); }

    void translate(const glm::vec3 & t);
    void updateScale(const glm::vec3 & t) { setScale(m_scale + t); };

    void setPosition(const glm::vec3 &new_pos);
    const glm::vec3 & getPosition() const { return m_position; }
    void setRotation(const glm::vec3 &new_rot);
    const glm::vec3 & getRotation() const { return m_rotation; }
    void setScale(const glm::vec3 &new_scale);
    const glm::vec3 & getScale() const { return m_scale; }

    Utils::Transform transform() const { return Utils::Transform(toEigen(m_translateMat * m_rotateMat)); }

    virtual void reset() {}
    void reset(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices) { m_meshes.clear(); m_meshes.emplace_back(new Mesh(vertices, indices, m_wire)); }

    virtual ModelType getType() const { return MODEL; }
    virtual std::string getTypeAsString() const { return "Object"; }
    virtual void editModel(const ModelParam &params) {}
    virtual ModelParam getParams() const { return ModelParam {}; }

    void setMaterial(const Material &material) { m_material = material; }

    Utils::Aabb aabb() const;
    Obb obb() const { return Obb(base_aabb(), transform()); }
    Obb uniformObb(float zoom) const { return Obb(uniform_aabb(zoom), transform()); }

    glm::mat4 rotation() const;

protected:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    bool m_wire { false };

private:
    Utils::Aabb base_aabb() const;
    Utils::Aabb uniform_aabb(float zoom) const;
    glm::mat4 scale() const { return glm::scale(glm::mat4(), m_scale); }

    void rotate(const glm::vec3 &rot);

    glm::vec3 m_position {0.f, 0.f, 0.f};
    glm::vec3 m_rotation {0.f, 0.f, 0.f};
    glm::vec3 m_scale {1.f, 1.f, 1.f};

    glm::mat4 m_translateMat { glm::mat4() };
    glm::mat4 m_rotateMat { glm::mat4() };

    Material m_material;
};


#endif //DAFT_ENGINE_MODEL_HPP
