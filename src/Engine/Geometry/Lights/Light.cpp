//
// Created by daftmat on 13/02/20.
//

#include "Light.hpp"

Light::Light( const glm::vec3& color ) : m_model{new Model( {}, {}, true )}, m_color{color} {}
