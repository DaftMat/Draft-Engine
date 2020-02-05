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
    //glm::vec3 Tangent;
    glm::vec2 TexCoords;
};

enum PrimitiveMesh {
    UV_SPHERE,
    ICO_SPHERE,
    CUBE_SPHERE
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    Mesh(PrimitiveMesh p, GLuint resolution);
    ~Mesh();

    void draw(const Shader &shader) const;

    void transform(glm::vec4 transform) { m_model *= transform; }
    glm::mat4 modelmatrix() { return m_model; }

private:
    void setupMesh();

    /// Mesh generation functions
    void createUVSphere(GLuint meridians, GLuint parallels);
    void createIcosahedron();

    /// Geometry
    std::vector<Vertex> m_vertices;
    /// Triangles
    std::vector<GLuint> m_indices;
    /// OpenGL
    GLuint m_VAO, m_VBO, m_EBO;

    glm::mat4 m_model;
};

#endif //DAFT_ENGINE_MESH_HPP
