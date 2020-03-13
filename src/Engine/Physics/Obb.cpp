/**
 * Created by mathis on 28/02/2020.
 */

#include "Obb.hpp"
glm::vec3 Obb::position() const {
    glm::mat4 model = toGlm( m_transform.matrix() );
    return glm::vec3{model[3].x, model[3].y, model[3].z};
}

glm::vec3 Obb::axis( int i ) const {
    glm::mat4 model = toGlm( m_transform.matrix() );
    return glm::vec3{model[i].x, model[i].y, model[i].z};
}