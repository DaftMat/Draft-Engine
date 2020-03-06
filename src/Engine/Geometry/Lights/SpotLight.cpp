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
    return result;
}

void SpotLight::editLight(const LightParam &params) {
    m_position = params.spotlight.position;
    m_direction = params.spotlight.direction;
    m_innerCutoff = params.spotlight.innerCutoff;
    m_outerCutoff = params.spotlight.outerCutoff;
    m_constant = params.spotlight.constant;
    m_linear = params.spotlight.linear;
    m_quadratic = params.spotlight.quadratic;
    m_model->setPosition(m_position);
    m_model->setRotation(params.spotlight.rotations);
    m_direction = m_model->rotation() * glm::vec4(m_direction, 0.f);
}

void SpotLight::update() {
    LightParam params = getParams();
    params.spotlight.position = m_model->getPosition();
    params.spotlight.rotations = m_model->getRotation();
    editLight(params);
}
