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

/** Model's settings type. */
union ModelParam {
    struct uvs_param {
        GLuint meridians; ///< UV Sphere's number of meridians.
        GLuint parallels; ///< UV Sphere's number of parallels.
    }uv_sphere;
    struct icos_param {
        GLuint subdivisions; ///< Icosahedron's number of subdivisions.
    }ico_sphere;
    struct cubs_param {
        GLuint resolution; ///< Cube Sphere's faces resolution.
    }cube_sphere;
    struct cube_param {
        GLuint resolution; ///< Cube's faces resolution.
    }cube;
};

/** Model's Type type. */
enum ModelType {
    MODEL, ///< No specific type.
    UV_SPHERE, ///< UVSphere.
    ICO_SPHERE, ///< IcoSphere.
    CUBE_SPHERE, ///< CubeSphere.
    CUBE ///< Cube.
};

/** Model Class.
 *  List of mesh with the same transformation.
 */
class Model {
public:
    /** Default constructor. */
    Model() = default;
    /** Constructor.
     *  Creates a one-mesh model given a geometry and a topology.
     * @param vertices - geometry of the mesh.
     * @param indices - topology of the mesh.
     * @param wire - is the created on-mesh model printed as wireframe.
     */
    Model(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, bool wire = false);
    virtual ~Model() { m_meshes.clear(); }

    Model(const Model &) = delete;
    Model & operator=(const Model &) = delete;

    /** Draws the model.
     *  Set the drawing shader for the current object and draws meshes in it.
     * @param shader - drawing shader.
     */
    void draw(const Shader &shader) const;

    /** Model's model matrix getter.
     *
     * @return the transformation matrix as a glm::mat4.
     */
    glm::mat4 model() const { return m_translateMat * m_rotateMat * scale(); }

    /** Translates the object.
     *
     * @param t - translation vector.
     */
    void translate(const glm::vec3 & t);
    /** "Translates" scale.
     *  > adds t to the scale.
     * @param t - scale vector.
     */
    void updateScale(const glm::vec3 & t) { setScale(m_scale + t); };

    /** Model's raw position setter. */
    void setPosition(const glm::vec3 &new_pos);
    /** Model's raw position getter. */
    const glm::vec3 & getPosition() const { return m_position; }
    /** Model's raw rotations setter. */
    void setRotation(const glm::vec3 &new_rot);
    /** Model's raw rotations getter. */
    const glm::vec3 & getRotation() const { return m_rotation; }
    /** Model's raw scale setter. */
    void setScale(const glm::vec3 &new_scale);
    /** Model's raw scale getter. */
    const glm::vec3 & getScale() const { return m_scale; }

    /** Model's model matrix getter.
     *
     * @return the transformation matrix as an Eigen::Transform.
     */
    Utils::Transform transform() const { return Utils::Transform(toEigen(m_translateMat * m_rotateMat)); }

    /** Resets the object */
    virtual void reset() {}
    /** Resets the object as a one-mesh with new geometry and topology.
     *
     * @param vertices - geometry
     * @param indices - topology
     */
    void reset(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices) { m_meshes.clear(); m_meshes.emplace_back(new Mesh(vertices, indices, m_wire)); }

    /** Model's type getter */
    virtual ModelType getType() const { return MODEL; }
    /** Model's type string getter */
    virtual std::string getTypeAsString() const { return "Object"; }
    /** Model's settings setter */
    virtual void editModel(const ModelParam &params) {}
    /** Model's settings getter */
    virtual ModelParam getParams() const { return ModelParam {}; }

    /** Model's material setter */
    void setMaterial(const Material &material) { m_material = material; }
    /** Model's material getter */
    const Material& material() const { return m_material; }
    /** Model's material ref setter */
    Material& material() { return m_material; }

    /** Model's Axis Aligned Bounding Box getter */
    Utils::Aabb aabb() const;
    /** Model's Oriented Bounding Box getter */
    Obb obb() const { return Obb(base_aabb(), transform()); }

    /** Model's rotation matrix getter */
    glm::mat4 rotation() const;

protected:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    bool m_wire { false };

private:
    Utils::Aabb base_aabb() const;
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
