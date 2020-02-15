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
}