/**
 * Created by mathis on 13/02/2020.
 */

#include "PointLight.hpp"

LightParam PointLight::getParams() const {
    LightParam result {};
    result.pointlight.position = m_position;
    result.pointlight.constant = m_constant;
    result.pointlight.linear = m_linear;
    result.pointlight.quadratic = m_quadratic;
    return result;
}

void PointLight::editLight(const LightParam &params) {
    m_position = params.pointlight.position;
    m_constant = params.pointlight.constant;
    m_linear = params.pointlight.linear;
    m_quadratic = params.pointlight.quadratic;
    m_model->setPosition(m_position);
}

void PointLight::setupModel() {
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
    for (int i = 0 ; i < 8 ; ++i) {
        vertex.Position = dir[i] * 0.4f;
        vertices.push_back(vertex);
        indices.push_back(0);
        indices.push_back(i+1);
    }

    m_model.reset(new Model(vertices, indices, true));
}
