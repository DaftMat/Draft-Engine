/**
 * Created by mathis on 01/02/2020.
 */

#ifndef DAFT_ENGINE_MESH_HPP
#define DAFT_ENGINE_MESH_HPP

#include <vector>
#include <opengl_stuff.h>

#include <Utils/types.hpp>
#include <Utils/adapters.hpp>

class Shader;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    explicit Mesh(bool grid = false) : m_vertices {}, m_indices {}, m_grid { grid } { setupMesh(); };
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, bool grid = false);
    ~Mesh();

    void draw() const;

    void reset(std::vector<Vertex> vertices, std::vector<GLuint> indices);

    const std::vector<Vertex>& vertices() const { return m_vertices; }
    const std::vector<GLuint>& indices() const { return m_indices; }

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

#endif //DAFT_ENGINE_MESH_HPP
