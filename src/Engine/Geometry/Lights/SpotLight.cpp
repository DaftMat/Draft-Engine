//
// Created by daftmat on 14/02/20.
//

#include "SpotLight.hpp"

LightParam SpotLight::getParams() const {
    LightParam result {};
    result.spotlight.position = m_position;
    result.spotlight.direction = m_direction;
    result.spotlight.innerCutoff = m_innerCutoff;
    result.spotlight.outerCutoff = m_outerCutoff;
    result.spotlight.constant = m_constant;
    result.spotlight.linear = m_linear;
    result.spotlight.quadratic = m_quadratic;
    result.spotlight.rotations = model().getRotation();
    return result;
}

void SpotLight::editLight(const LightParam &params) {
    m_position = params.spotlight.position;
    m_innerCutoff = params.spotlight.innerCutoff;
    m_outerCutoff = params.spotlight.outerCutoff;
    m_constant = params.spotlight.constant;
    m_linear = params.spotlight.linear;
    m_quadratic = params.spotlight.quadratic;
    m_model->setPosition(m_position);
    m_model->setRotation(params.spotlight.rotations);
    m_direction = m_model->rotation() * glm::vec4(m_baseDirection, 0.f);
}

void SpotLight::update() {
    LightParam params = getParams();
    params.spotlight.position = m_model->getPosition();
    params.spotlight.rotations = m_model->getRotation();
    editLight(params);
}

void SpotLight::setupModel() {
    // origin of light
    std::vector<glm::vec3> dir {
            { 0.f, 0.f, 1.f },  // front
            { 0.f, 0.f,-1.f },  // back
            { 0.f, 1.f, 0.f },  // top
            { 0.f,-1.f, 0.f },  // bottom
            { 1.f, 0.f, 0.f },  // right
            {-1.f, 0.f, 0.f }   // left
    };

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Vertex vertex {};
    vertices.push_back(vertex);

    for (int i = 0 ; i < 6 ; ++i) {
        vertex.Position = dir[i] * 0.5f;
        vertices.push_back(vertex);
        indices.push_back(0);
        indices.push_back(i+1);
    }

    float innerDist = glm::tan(glm::radians(m_innerCutoff / 2.f)) * 4.f;
    float outerDist = glm::tan(glm::radians(m_outerCutoff / 2.f)) * 4.f;

    glm::vec3 pointTo = m_baseDirection * 4.f;
    glm::vec3 axisA { m_baseDirection.y, m_baseDirection.z, m_baseDirection.x };
    glm::vec3 axisB = glm::cross(m_baseDirection, axisA);

    /// inner circle
    vertex.Position = pointTo + axisA * innerDist;
    vertices.push_back(vertex);
    indices.push_back(0);
    indices.push_back(7);

    vertex.Position = pointTo + glm::normalize(axisA + axisB) * innerDist;
    vertices.push_back(vertex);
    indices.push_back(7);
    indices.push_back(8);

    vertex.Position = pointTo + axisB * innerDist;
    vertices.push_back(vertex);
    indices.push_back(0);
    indices.push_back(9);
    indices.push_back(8);
    indices.push_back(9);

    vertex.Position = pointTo + glm::normalize(axisB - axisA) * innerDist;
    vertices.push_back(vertex);
    indices.push_back(9);
    indices.push_back(10);

    vertex.Position = pointTo - axisA * innerDist;
    vertices.push_back(vertex);
    indices.push_back(0);
    indices.push_back(11);
    indices.push_back(10);
    indices.push_back(11);

    vertex.Position = pointTo + glm::normalize(-axisA - axisB) * innerDist;
    vertices.push_back(vertex);
    indices.push_back(11);
    indices.push_back(12);

    vertex.Position = pointTo - axisB * innerDist;
    vertices.push_back(vertex);
    indices.push_back(0);
    indices.push_back(13);
    indices.push_back(12);
    indices.push_back(13);

    vertex.Position = pointTo + glm::normalize(axisA - axisB) * innerDist;
    vertices.push_back(vertex);
    indices.push_back(13);
    indices.push_back(14);
    indices.push_back(14);
    indices.push_back(7);

    ///outer circle
    vertex.Position = pointTo + axisA * outerDist;
    vertices.push_back(vertex);
    indices.push_back(0);
    indices.push_back(15);

    vertex.Position = pointTo + glm::normalize(axisA + axisB) * outerDist;
    vertices.push_back(vertex);
    indices.push_back(15);
    indices.push_back(16);

    vertex.Position = pointTo + axisB * outerDist;
    vertices.push_back(vertex);
    indices.push_back(0);
    indices.push_back(17);
    indices.push_back(16);
    indices.push_back(17);

    vertex.Position = pointTo + glm::normalize(axisB - axisA) * outerDist;
    vertices.push_back(vertex);
    indices.push_back(17);
    indices.push_back(18);

    vertex.Position = pointTo - axisA * outerDist;
    vertices.push_back(vertex);
    indices.push_back(0);
    indices.push_back(19);
    indices.push_back(18);
    indices.push_back(19);

    vertex.Position = pointTo + glm::normalize(-axisA - axisB) * outerDist;
    vertices.push_back(vertex);
    indices.push_back(19);
    indices.push_back(20);

    vertex.Position = pointTo - axisB * outerDist;
    vertices.push_back(vertex);
    indices.push_back(0);
    indices.push_back(21);
    indices.push_back(20);
    indices.push_back(21);

    vertex.Position = pointTo + glm::normalize(axisA - axisB) * outerDist;
    vertices.push_back(vertex);
    indices.push_back(21);
    indices.push_back(22);
    indices.push_back(22);
    indices.push_back(15);

    m_model->reset(vertices, indices);
}
