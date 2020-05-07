/**
 * Created by mathis on 06/05/2020.
 */

#ifndef DAFT_ENGINE_MESHOBJECT_HPP
#define DAFT_ENGINE_MESHOBJECT_HPP


#include <memory>
#include <src/Engine/Cameras/camera.h>
#include "Mesh.hpp"

/** Mesh wrapper.
 * Object that wraps Mesh class so it can be extended.
 */
class MeshObject {
public:
    /** Constructor.
     *
     * @param grid - drew as wireframes.
     */
    explicit MeshObject(bool grid = false) : m_mesh { new Mesh(grid) } {}
    ~MeshObject() { m_mesh.reset(); }

    /** Constructor with vertices and indices.
     *
     * @param vertices - vertices of the inner Mesh.
     * @param indices - indices of the inner Mesh.
     * @param grid - drew as wireframes.
     */
    MeshObject(std::vector<Mesh::Vertex> vertices, std::vector<GLuint> indices, bool grid = false)
        : m_mesh { new Mesh(std::move(vertices), std::move(indices), grid) } {}

    /** draws the inner Mesh.
     *
     */
    void draw() const { m_mesh->draw(); }

    virtual void draw(const Shader &shader) {}

    /** Added function : used for animated meshes.
     *
     */
     virtual void update(float dt) {}

     virtual glm::mat4 model() const { return glm::mat4 {}; }

     virtual float getSetting() const { return 0.f; }

    /** resets the inner Mesh.
     *
     * @param vertices - new vertices of the inner Mesh.
     * @param indices - new indices of the inner Mesh.
     */
    void reset(std::vector<Mesh::Vertex> vertices, std::vector<GLuint> indices)
    { m_mesh->reset(std::move(vertices), std::move(indices)); }

    /** Geometry const ref getter.
     * c.f. Vertex
     * @return geometry - list of vertices.
     */
    const std::vector<Mesh::Vertex>& vertices() const { return m_mesh->vertices(); }

    /** Topology const ref getter.
     *
     * @return topology - list of indices.
     */
    const std::vector<GLuint>& indices() const { return m_mesh->indices(); }

    /** Geometry copy getter.
     * used in Raytracer.
     * @return copied geometry.
     */
    std::vector<Mesh::Vertex> getVertices() const { return m_mesh->vertices(); }

    /** Topology copy getter.
     * used in Raytracer.
     * @return copied topology.
     */
    std::vector<GLuint> getIndices() const { return m_mesh->indices(); }

    /** Aligned Axis Bounding Box getter.
     * calculate untransformed mesh's AABB.
     * @return AABB of the mesh.
     */
    Utils::Aabb aabb() const { return m_mesh->aabb(); }

    /** Number of triangles getter.
     *
     * @return number of triangles in the mesh.
     */
    GLuint getSize() const { return m_mesh->getSize(); }

protected:
    std::unique_ptr<Mesh> m_mesh;
};


#endif //DAFT_ENGINE_MESHOBJECT_HPP
