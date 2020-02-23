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
    m_translateMat = glm::translate(m_translateMat, t);
    m_position = new_pos;
}

void Model::setRotation(const glm::vec3 &new_rot) {
    glm::vec3 r = new_rot - m_rotation;
    if (glm::length(r) < glm::epsilon<float>()) return;
    m_rotation = new_rot;
}

void Model::setScale(const glm::vec3 &new_scale) {
    if (m_scale == new_scale)   return;
    m_scale = new_scale;
}

glm::mat4 Model::rotation() const {
    glm::vec3 x { 1.f, 0.f, 0.f };
    glm::vec3 y { 0.f, 1.f, 0.f };
    glm::vec3 z { 0.f, 0.f, 1.f };
    glm::mat4 res;
    res = glm::rotate(res, glm::radians(m_rotation.x), x);
    res = glm::rotate(res, glm::radians(m_rotation.y), y);
    res = glm::rotate(res, glm::radians(m_rotation.z), z);
    return res;
}

void Model::translate(const glm::vec3 &t) {
    m_translateMat = glm::translate(m_translateMat, t);
    m_position += t;
}
