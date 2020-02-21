//
// Created by daftmat on 10/02/20.
//

#include "Model.hpp"

void Model::draw(const Shader &shader) const {
    shader.use();
    shader.setMat4("model", model());

    for (auto & mesh : m_meshes) {
        mesh->draw();
    }
}

void Model::setPosition(const glm::vec3 &new_pos) {
    glm::vec3 t = new_pos - m_position;
    if (glm::length(t) < glm::epsilon<float>()) return;
    translate(t);
    m_position = new_pos;
}

void Model::setRotation(const glm::vec3 &new_rot) {
    glm::vec3 r = new_rot - m_rotation;
    if (glm::length(r) < glm::epsilon<float>()) return;
    rotate(r, glm::length(r));
    m_rotation = new_rot;
}

void Model::setScale(const glm::vec3 &new_scale) {
    if (m_scale == new_scale)   return;
    m_scale = new_scale;
    m_scaleMat = glm::scale(glm::mat4(), m_scale);
}
