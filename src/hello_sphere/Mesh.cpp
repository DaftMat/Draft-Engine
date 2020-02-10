/**
 * Created by mathis on 01/02/2020.
 */

#include "Mesh.hpp"
#include "Shader.hpp"
#include <cassert>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
: m_vertices { std::move(vertices) }, m_indices { std::move(indices) } {
    setupMesh();
}

Mesh::Mesh(PrimitiveMesh p, const std::vector<GLuint> & params) {
    switch (p) {
        case UV_SPHERE:
            assert(params.size() >= 2);
            createUVSphere(params[0], params[1]);
            break;
        default:
            break;
    }
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    m_vertices.clear();
    m_indices.clear();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    /// open
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    /// in vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
    /// in vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    /// in vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    /// release
    glBindVertexArray(0);
}

void Mesh::draw(const Shader &shader) const {
    shader.setMat4("model", m_model);

    /// HERE COMES THE BUG (glBoindVertexArray returns "Invalid Operation")
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

/// Primitives
void Mesh::createUVSphere(GLuint stacks, GLuint sectors) {
    Vertex vertex;

    /// Vertices
    float z, xy;

    float sectorStep = 2.f * glm::pi<float>() / sectors;
    float stackStep = glm::pi<float>() / stacks;
    float sectorAngle, stackAngle;

    for (GLuint i = 0 ; i <= stacks ; ++i) {
        stackAngle = glm::pi<float>() / 2.f - i * stackStep;
        xy = glm::cos(stackAngle);
        z = glm::sin(stackAngle);

        for (GLuint j = 0 ; j <= sectors ; ++j) {
            sectorAngle = j * sectorStep;

            vertex.Position = glm::vec3(xy * glm::cos(sectorAngle), xy * glm::sin(sectorAngle), z);
            vertex.Normal = glm::vec3(xy * glm::cos(sectorAngle), xy * glm::sin(sectorAngle), z);
            vertex.TexCoords = glm::vec2((float)j / sectors, (float)i / stacks);

            m_vertices.push_back(vertex);
        }
    }

    /// Triangles
    GLuint k1, k2;
    for (GLuint i = 0 ; i < stacks ; ++i) {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;
        for (GLuint j = 0 ; j < sectors ; ++j, ++k1, ++k2) {
            if (i) {
                m_indices.push_back(k1);
                m_indices.push_back(k2);
                m_indices.push_back(k1 + 1);
            }
            if (i != (stacks - 1)) {
                m_indices.push_back(k1 + 1);
                m_indices.push_back(k2);
                m_indices.push_back(k2 + 1);
            }
        }
    }
}