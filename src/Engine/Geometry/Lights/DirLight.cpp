//
// Created by daftmat on 14/02/20.
//

#include "DirLight.hpp"

LightParam DirLight::getParams() const {
    LightParam result {};
    result.dirlight.direction = m_direction;
    return result;
}

void DirLight::editLight(const LightParam &params) {
    m_model->setRotation(params.dirlight.rotations);
    m_direction = m_model->rotation() * glm::vec4(m_basedirection, 0.f);
}

void DirLight::update() {
    LightParam params = getParams();
    params.dirlight.rotations = m_model->getRotation();
    editLight(params);
}
