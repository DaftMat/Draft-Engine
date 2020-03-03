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

    float scale = 0.01f;
    float dist = 4.f;

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
    vertex.Position = tangent * dist * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(tangent+bitangent) * dist * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = bitangent * dist * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(bitangent-tangent) * dist * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = -tangent * dist * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(-tangent-bitangent) * dist * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = -bitangent * dist * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = glm::normalize(tangent-bitangent) * dist * scale + normal;
    vertices.push_back(vertex);
    vertex.Position = normal + (normal * 8.f * scale);
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

void TranslateGizmo::move(float xpos, float ypos, Model &model, const glm::mat4 &projection, const glm::mat4 &view) {
    glm::vec3 dir = getDir(model.rotation());
    float xoffset = xpos - m_xmouse;
    float yoffset = ypos - m_ymouse;
    m_xmouse = xpos;
    m_ymouse = ypos;
    glm::vec2 mousevec = { xoffset, yoffset };
    glm::vec2 dirscreen = glm::normalize(glm::vec2(projection * view * glm::vec4(dir, 1.0)));
    float angle = glm::dot(mousevec, dirscreen);
    std::cout << angle << std::endl;
    std::cout << dir.x << std::endl;
    model.translate(angle * dir / 100.f);
    setTransform(model.transform());
}

glm::vec3 TranslateGizmo::getDir(const glm::mat4 &rotation) {
    switch (m_selected) {
        case XSELEC:
            return glm::normalize(glm::vec3(rotation * glm::vec4(glm::vec3(1.f, 0.f, 0.f), 1.0)));
        case YSELEC:
            return glm::normalize(glm::vec3(rotation * glm::vec4(glm::vec3(0.f, 1.f, 0.f), 1.0)));
        case ZSELEC:
            return glm::normalize(glm::vec3(rotation * glm::vec4(glm::vec3(0.f, 0.f, 1.f), 1.0)));
        default:
            return glm::vec3(0.f, 0.f, 0.f);
    }
}