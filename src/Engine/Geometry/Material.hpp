/**
 * Created by mathis on 08/03/2020.
 */

#ifndef DAFT_ENGINE_MATERIAL_HPP
#define DAFT_ENGINE_MATERIAL_HPP

#include <opengl_stuff.h>

class Material {
public:
    Material() : m_albedo { 0.8f, 0.f, 0.8f }, m_specular { 0.9f, 0.9f, 0.9f }, m_roughness { 0.1f }, m_ao { 1.f }, m_ior { 1.777 } {}
    Material(const glm::vec3 &albedo, const glm::vec3 &specular, float roughness, float ao, float ior) :
    m_albedo { albedo },
    m_specular { specular },
    m_roughness { roughness },
    m_ao { ao }, m_ior { ior } {}

    glm::vec3 &albedo() { return m_albedo; }
    const glm::vec3 &albedo() const { return m_albedo; }
    glm::vec3 &specular() { return m_specular; }
    const glm::vec3 &specular() const { return m_specular; }
    float &roughness() { return m_roughness; }
    float roughness() const { return m_roughness; }
    float &ambientOcclusion() { return m_ao; }
    float ambientOcclusion() const { return m_ao; }
    float &IOR() { return m_ior; }
    float IOR() const { return m_ior; }

private:
    glm::vec3 m_albedo;
    glm::vec3 m_specular;
    float m_roughness;
    float m_ao;
    float m_ior;
};


#endif //DAFT_ENGINE_MATERIAL_HPP
