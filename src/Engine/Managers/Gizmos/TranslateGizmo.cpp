/**
 * Created by mathis on 01/03/2020.
 */

#include "TranslateGizmo.hpp"

void TranslateGizmo::init_models() {
    m_Xmodel.reset(init_arrow(glm::vec3(1.f, 0.f, 0.f)));
    m_Ymodel.reset(init_arrow(glm::vec3(0.f, 1.f, 0.f)));
    m_Zmodel.reset(init_arrow(glm::vec3(0.f, 0.f, 1.f)));
 }

Model * TranslateGizmo::init_arrow(const glm::vec3 &normal) {
    glm::vec3 tangent {
        normal.z,
        normal.x,
        normal.y
    };
    glm::vec3 bitangent = glm::cross(normal, tangent);
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    float scale = 0.025f;

    Vertex vertex {};
    vertex.Position = tangent * scale;
    vertices.push_back(vertex);
    vertex.Position += normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(tangent+bitangent) * scale;
    vertices.push_back(vertex);
    vertex.Position += normal;
    vertices.push_back(vertex);
    vertex.Position = bitangent * scale;
    vertices.push_back(vertex);
    vertex.Position += normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(bitangent-tangent) * scale;
    vertices.push_back(vertex);
    vertex.Position += normal;
    vertices.push_back(vertex);
    vertex.Position = -tangent * scale;
    vertices.push_back(vertex);
    vertex.Position += normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(-tangent-bitangent) * scale;
    vertices.push_back(vertex);
    vertex.Position += normal;
    vertices.push_back(vertex);
    vertex.Position = -bitangent * scale;
    vertices.push_back(vertex);
    vertex.Position += normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(tangent-bitangent) * scale;
    vertices.push_back(vertex);
    vertex.Position += normal;
    vertices.push_back(vertex);
    vertex.Position = tangent * 2.f * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(tangent+bitangent) * 2.f * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = bitangent * 2.f * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(bitangent-tangent) * 2.f * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = -tangent * 2.f * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(-tangent-bitangent) * 2.f * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = -bitangent * 2.f * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(tangent-bitangent) * 2.f * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = normal + (normal * 4.f * scale);
    vertices.push_back(vertex);
    for (int i = 0 ; i < 16 ; i += 2) {
        indices.push_back(i);
        indices.push_back((i+3)%16);
        indices.push_back(i+1);
        indices.push_back(i);
        indices.push_back((i+2)%16);
        indices.push_back((i+3)%16);
    }
    for (int i = 16 ; i < 24 ; ++i) {
        indices.push_back(i);
        indices.push_back(((i+1)%8)+16);
        indices.push_back(24);
    }
    return new Model(vertices, indices);
}

void TranslateGizmo::scale(float scale) {
    m_Xmodel->setScale(glm::vec3(scale));
    m_Ymodel->setScale(glm::vec3(scale));
    m_Zmodel->setScale(glm::vec3(scale));
}
