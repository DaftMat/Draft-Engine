/**
 * Created by mathis on 01/02/2020.
 */

#include "Mesh.hpp"
#include "Shader.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
: m_vertices { std::move(vertices) }, m_indices { std::move(indices) } {
    setupMesh();
}

Mesh::Mesh(PrimitiveMesh p, GLuint resolution) {
    switch (p) {
        case UV_SPHERE:
            createUVSphere(resolution, resolution);
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

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

/// Primitives
void Mesh::createUVSphere(GLuint meridians, GLuint parallels) {
    Vertex vertex;

    /// Vertices
    float z, xy;

    float parallelStep = 2.f * glm::pi<float>() / parallels;
    float meridianStep = glm::pi<float>() / meridians;
    float parallelAngle, meridianAngle;

    for (GLuint i = 0 ; i <= parallels ; ++i) {
        parallelAngle = glm::pi<float>() / 2.f - i * parallelStep;
        xy = glm::cos(parallelAngle);
        z = glm::sin(parallelAngle);

        for (GLuint j = 0 ; j <= meridians ; ++j) {
            meridianAngle = j * meridianStep;

            vertex.Position = glm::vec3(xy * glm::cos(meridianAngle), xy * glm::sin(meridianAngle), z);
            vertex.Normal = glm::vec3(xy * glm::cos(meridianAngle), xy * glm::sin(meridianAngle), z);
            vertex.TexCoords = glm::vec2((float)j / meridians, (float)i / parallels);

            m_vertices.push_back(vertex);
        }
    }

    /// Triangles
    GLuint k1, k2;
    for (GLuint i = 0 ; i < parallels ; ++i) {
        k1 = i * (meridians + 1);
        k2 = k1 + meridians + 1;
        for (GLuint j = 0 ; j < meridians ; ++j) {
            if (!i) {
                m_indices.push_back(k1);
                m_indices.push_back(k2);
                m_indices.push_back(k1 + 1);
            }
            if (i != (parallels - 1)) {
                m_indices.push_back(k1 + 1);
                m_indices.push_back(k2);
                m_indices.push_back(k2 + 1);
            }
        }
    }
}