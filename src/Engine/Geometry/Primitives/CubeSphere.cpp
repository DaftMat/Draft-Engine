/**
 * Created by mathis on 13/02/2020.
 */

#include "CubeSphere.hpp"

void CubeSphere::editModel(const ModelParam &params) {
    m_resolution = params.cube_sphere.resolution;
    if (m_resolution < 2)
        m_resolution = 2;
}

ModelParam CubeSphere::getParams() const {
    ModelParam result {};
    result.cube_sphere.resolution = m_resolution;
    return result;
}

void CubeSphere::setupCubeSphere() {
    std::vector<glm::vec3> directions = {
        glm::vec3(0.f, 0.f, 1.f),   //front
        glm::vec3(0.f, 0.f,-1.f),   //back
        glm::vec3(0.f, 1.f, 0.f),   //up
        glm::vec3(0.f,-1.f, 0.f),   //bottom
        glm::vec3(1.f, 0.f, 0.f),   //left
        glm::vec3(-1.f,0.f, 0.f)    //right
    };

    m_meshes.clear();

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Vertex vertex;
    glm::vec3 axisA, axisB;

    for (GLuint i = 0 ; i < 6 ; ++i) {
        // Rotate the direction
        axisA = glm::vec3(directions[i].y, directions[i].z, directions[i].x);
        axisB = glm::cross(directions[i], axisA);
        for (GLuint y = 0; y < m_resolution; ++y) {
            for (GLuint x = 0; x < m_resolution; ++x) {
                ///Vertex
                GLuint index = x + y * m_resolution;
                glm::vec2 percent = glm::vec2(x, y) / (m_resolution - 1.f);
                glm::vec3 pointOnUnitCube =
                        directions[i]
                        + (percent.x - 0.5f) * 2.f * axisA
                        + (percent.y - 0.5f) * 2.f * axisB;
                glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

                vertex.Position = vertex.Normal = pointOnUnitSphere;
                vertex.TexCoords = percent;
                vertices.push_back(vertex);

                ///Triangles
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

CubeSphere::CubeSphere(GLuint resolution) : m_resolution { resolution } {
    setupCubeSphere();
    m_aabb.min = { -1.f, -1.f, -1.f };
    m_aabb.max = {  1.f,  1.f,  1.f };
}
