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
}
