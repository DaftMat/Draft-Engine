/**
 * Created by mathis on 01/02/2020.
 */

#ifndef DAFT_ENGINE_MESH_HPP
#define DAFT_ENGINE_MESH_HPP

#include <vector>
#include <opengl_stuff.h>

class Shader;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    Mesh() : m_vertices {}, m_indices {} { setupMesh(); };
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    ~Mesh();

    void draw() const;

    void reset();

    std::vector<Vertex> vertices() const { return m_vertices; }
    std::vector<GLuint> indices() const { return m_indices; }

private:
    void setupMesh();
    void deleteMesh();

    /// Geometry
    std::vector<Vertex> m_vertices;
    /// Triangles
    std::vector<GLuint> m_indices;
    /// OpenGL
    GLuint m_VAO, m_VBO, m_EBO;
};

#endif //DAFT_ENGINE_MESH_HPP
