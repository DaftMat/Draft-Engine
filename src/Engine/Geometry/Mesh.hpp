//
// Created by mathis on 01/02/2020.
//

#ifndef DAFT_ENGINE_MESH_HPP
#define DAFT_ENGINE_MESH_HPP

#include <opengl_stuff.h>
#include <vector>

#include <Utils/adapters.hpp>
#include <Utils/types.hpp>

class Shader;

/** Defines a mesh with a geometry and a topology */
class Mesh
{
  public:
    /** defines all the information needed in a vertex. */
    struct Vertex {
        glm::vec3 Position;  ///< world position of the vertex.
        glm::vec3 Normal;    ///< normal to the vertex.
        glm::vec2 TexCoords; ///< texture coordinates of the vertex.
    };

    /** Constructor.
     *  Creates an empty mesh.
     * @param grid - will draw the mesh with lines if true, with triangles otherwise.
     */
    explicit Mesh( bool grid = false ) : m_vertices{}, m_indices{}, m_grid{grid} { setupMesh(); };
    /** Constructor.
     *
     * @param vertices - geometry of the mesh.
     * @param indices - topology of the mesh.
     * @param grid - will draw the mesh with lines if true, with triangles otherwise.
     */
    Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices, bool grid = false );
    ~Mesh();

    /** Draws the mesh.
     * draw the mesh on current OpenGL context.
     */
    void draw() const;

    /** Reset mesh.
     *  Reassign new geometry and topology to the mesh.
     * @param vertices - new geometry.
     * @param indices - new topology.
     */
    void reset( std::vector<Vertex> vertices, std::vector<GLuint> indices );

    /** Geometry getter.
     * c.f. Vertex
     * @return geometry - list of vertices.
     */
    const std::vector<Vertex>& vertices() const { return m_vertices; }

    /** Topology getter.
     *
     * @return topology - list of indices.
     */
    const std::vector<GLuint>& indices() const { return m_indices; }

    /** Aligned Axis Bounding Box getter.
     * calculate untransformed mesh's AABB.
     * @return AABB of the mesh.
     */
    Utils::Aabb aabb() const;

  private:
    void setupMesh();
    void deleteMesh();

    /// Geometry
    std::vector<Vertex> m_vertices;
    /// Triangles
    std::vector<GLuint> m_indices;
    /// OpenGL
    GLuint m_VAO, m_VBO, m_EBO;
    bool m_grid;
};

#endif // DAFT_ENGINE_MESH_HPP
