/**
 * Created by mathis on 05/03/2020.
 */

#include "Cube.hpp"

void Cube::setupCube() {
    std::vector<glm::vec3> directions {
            { 0.f, 0.f, 1.f },  // front
            { 0.f, 0.f,-1.f },  // back
            { 0.f, 1.f, 0.f },  // top
            { 0.f,-1.f, 0.f },  // bottom
            { 1.f, 0.f, 0.f },  // right
            {-1.f, 0.f, 0.f }   // left
    };
    m_meshes.clear();
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Vertex vertex;
    glm::vec3 axisA, axisB;

    for (GLuint i = 0 ; i < 6 ; ++i) {
        axisA = { directions[i].y, directions[i].z, directions[i].x };
        axisB = glm::cross(directions[i], axisA);
        for(GLuint y = 0 ; y < m_resolution ; ++y) {
            for (GLuint x = 0 ; x < m_resolution ; ++x) {
                GLuint index = x + y * m_resolution;
                glm::vec2 percent = glm::vec2(x, y) / (m_resolution - 1.f);

                vertex.Position = directions[i]
                        + (percent.x - 0.5f) * 2.f * axisA
                        + (percent.y - 0.5f) * 2.f * axisB;
                vertex.Normal = directions[i];
                vertices.push_back(vertex);

                if (x != m_resolution - 1 && y != m_resolution - 1) {
                    indices.push_back(index);
                    indices.push_back(index + m_resolution + 1);
                    indices.push_back(index + m_resolution);

                    indices.push_back(index);
                    indices.push_back(index + 1);
                    indices.push_back(index + m_resolution + 1);
                }
            }
        }
        m_meshes.emplace_back(new Mesh(vertices, indices));
        vertices.clear();
        indices.clear();
    }
}

void Cube::editModel(const ModelParam &params) {
    m_resolution = params.cube.resolution;
    if (m_resolution < 2)
        m_resolution = 2;
}

ModelParam Cube::getParams() const {
    ModelParam params {};
    params.cube.resolution = m_resolution;
    return params;
}
