//
// Created by daftmat on 10/02/20.
//

#include "UVSphere.hpp"

void UVSphere::setupUvSphere() {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Vertex vertex;

    /// Vertices
    float z, xy;

    float sectorStep = (2.f * glm::pi<float>()) / m_sectors;
    float stackStep = glm::pi<float>() / m_stacks;
    float sectorAngle, stackAngle;

    for (GLuint i = 0 ; i <= m_stacks ; ++i) {
        stackAngle = glm::pi<float>() / 2.f - i * stackStep;
        xy = glm::cos(stackAngle);
        z = glm::sin(stackAngle);

        for (GLuint j = 0 ; j <= m_sectors ; ++j) {
            sectorAngle = j * sectorStep;

            vertex.Position = glm::vec3(xy * glm::cos(sectorAngle), xy * glm::sin(sectorAngle), z);
            vertex.Normal = glm::vec3(xy * glm::cos(sectorAngle), xy * glm::sin(sectorAngle), z);
            vertex.TexCoords = glm::vec2((float)j / m_sectors, (float)i / m_stacks);

            vertices.push_back(vertex);
        }
    }

    /// Triangles
    GLuint k1, k2;
    for (GLuint i = 0 ; i < m_stacks ; ++i) {
        k1 = i * (m_sectors + 1);
        k2 = k1 + m_sectors + 1;
        for (GLuint j = 0 ; j < m_sectors ; ++j, ++k1, ++k2) {
            if (i) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (m_stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    m_meshes.clear();
    m_meshes.emplace_back(vertices, indices);
}
