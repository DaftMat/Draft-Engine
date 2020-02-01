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
    //glm::vec2 TexCoords;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    void draw(const Shader &shader) const;

private:
    /// Geometry
    std::vector<Vertex> m_vertices;
    /// Triangles
    std::vector<GLuint> m_indices;
    /// OpenGL
    GLuint m_VAO, m_VBO, m_EBO;
    void setupMesh();
};

/// Mesh generation function
Mesh createUVSphere(int meridians, int parallels);
Mesh createIcosahedron();

#endif //DAFT_ENGINE_MESH_HPP
