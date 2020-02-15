//
// Created by daftmat on 14/02/20.
//

#include "DirLight.hpp"

LightParam DirLight::getParams() const {
    LightParam result {};
    result.dirlight.direction = m_direction;
    return result;
}
