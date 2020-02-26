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
    transformAABB();
}

void Model::setRotation(const glm::vec3 &new_rot) {
    glm::vec3 r = new_rot - m_rotation;
    if (glm::length(r) < glm::epsilon<float>()) return;
    m_rotation = new_rot;
    transformAABB();
}

void Model::setScale(const glm::vec3 &new_scale) {
    if (m_scale == new_scale)   return;
    glm::vec3 t = new_scale - m_scale;
    m_scale = new_scale;
    transformAABB();
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
    transformAABB();
}

bool Model::isIntersected(const Ray &ray, float &dist) {
    float tMin { 0.f };
    float tMax { 100000.f };

    for (int a = 0; a < 3; ++a) {
        float invD = 1.0f / ray.direction[a];
        float t0 = (m_aabb.min[a] - ray.position[a]) * invD;
        float t1 = (m_aabb.max[a] - ray.position[a]) * invD;
        if (invD < 0.0f) {
            float temp = t1;
            t1 = t0;
            t0 = temp;
        }

        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;

        if (tMax <= tMin)
            return false;
    }

    dist = tMin;
    return true;
}

bool Model::isOBBIntersected(const Ray &ray, float &dist) {
    float tMin { 0.f };
    float tMax { 100000.f };

    glm::mat4 modelMat = m_translateMat * rotation();
    glm::vec3 OBBPosition{
        modelMat[3].x,
        modelMat[3].y,
        modelMat[3].z
    };
    glm::vec3 delta = OBBPosition - ray.position;

    for (int i = 0 ; i < 3 ; ++i) {
        glm::vec3 axis {
            modelMat[i].x,
            modelMat[i].y,
            modelMat[i].z
        };
        float e = glm::dot(axis, delta);
        float f = glm::dot(ray.direction, axis);

        if (glm::abs(f) > 0.001f) {
            float t1 = (e + m_aabb.min.x) / f;
            float t2 = (e + m_aabb.max.y) / f;

            if (t1 > t2) std::swap(t1, t2);
            if (t2 < tMax) tMax = t2;
            if (t1 > tMin) tMin = t1;

            if (tMax < tMin) return false;
        } else if (-e + m_aabb.min[i] > 0.f || -e + m_aabb.max[i] < 0.f) {
            return false;
        }
    }

    dist = tMin;
    return true;
}

void Model::transformAABB() {
    m_aabb.min = std::numeric_limits<glm::vec3>::infinity();
    m_aabb.max = -std::numeric_limits<glm::vec3>::infinity();
    for (const auto &mesh : m_meshes) {
        for (const auto &vertex : mesh->vertices()) {
            glm::vec3 v = rotation() * scale() * glm::vec4(vertex.Position, 1.0);
            m_aabb.min = {
                    glm::min(v.x, m_aabb.min.x),
                    glm::min(v.y, m_aabb.min.y),
                    glm::min(v.z, m_aabb.min.z)
            };
            m_aabb.max = {
                    glm::max(v.x, m_aabb.max.x),
                    glm::max(v.y, m_aabb.max.y),
                    glm::max(v.z, m_aabb.max.z)
            };
        }
    }
    m_aabb.min += m_position;
    m_aabb.max += m_position;
}