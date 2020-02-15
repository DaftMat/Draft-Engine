//
// Created by daftmat on 13/02/20.
//

#include "Light.hpp"

Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) :
    m_ambient { ambient }, m_diffuse { diffuse }, m_specular { specular } {}