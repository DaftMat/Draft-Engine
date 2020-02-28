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
    m_position = new_pos;
    m_transform.translation() = toEigen(m_position);
}

void Model::setRotation(const glm::vec3 &new_rot) {
    glm::vec3 rot = new_rot - m_rotation;
    m_rotation = new_rot;
    Utils::Transform r, rx, ry, rz;
    rx = Eigen::AngleAxisf(glm::radians(rot.x), Eigen::Vector3f(1.f, 0.f, 0.f));
    ry = Eigen::AngleAxisf(glm::radians(rot.y), Eigen::Vector3f(0.f, 1.f, 0.f));
    rz = Eigen::AngleAxisf(glm::radians(rot.z), Eigen::Vector3f(0.f, 0.f, 1.f));
    r = rx * ry * rz;
    m_transform.rotate(r.rotation());
}

void Model::setScale(const glm::vec3 &new_scale) {
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
    setPosition(m_position + t);
}

Utils::Aabb Model::aabb() const {
    Utils::Aabb aabb;
    for (const auto &m : m_meshes) {
        aabb.extend(m->aabb());
    }
    if (aabb.isEmpty()) return aabb;
    Utils::Aabb res;
    for (int i = 0 ; i < 8 ; ++i) {
        res.extend(modelEigen() * aabb.corner(Utils::Aabb::CornerType(i)));
    }
    return res;
}