/**
 * Created by mathis on 08/03/2020.
 */

#ifndef DAFT_ENGINE_MATERIAL_HPP
#define DAFT_ENGINE_MATERIAL_HPP

#include <opengl_stuff.h>

class Material {
public:
    Material() : m_albedo { 0.9f, 0.9f, 0.9f }, m_diffuse { 0.8f, 0.f, 0.f }, m_metalness { 0.9f }, m_roughness { 0.1f }, m_ao { 1.f }, m_ior { 1.777 } {}
    Material(const glm::vec3 &albedo, const glm::vec3 &diffuse, float metalness, float roughness, float ao, float ior) :
    m_albedo { albedo },
    m_diffuse { diffuse },
    m_metalness { metalness },
    m_roughness { roughness },
    m_ao { ao }, m_ior { ior } {}

    glm::vec3 &albedo() { return m_albedo; }
    const glm::vec3 &albedo() const { return m_albedo; }
    glm::vec3 &diffuse() { return m_diffuse; }
    const glm::vec3 &diffuse() const { return m_diffuse; }
    float &metalness() { return m_metalness; }
    float metalness() const { return m_metalness; }
    float &roughness() { return m_roughness; }
    float roughness() const { return m_roughness; }
    float &ambientOcclusion() { return m_ao; }
    float ambientOcclusion() const { return m_ao; }
    float &IOR() { return m_ior; }
    float IOR() const { return m_ior; }

private:
    glm::vec3 m_albedo;
    glm::vec3 m_diffuse;
    float m_metalness;
    float m_roughness;
    float m_ao;
    float m_ior;
};


#endif //DAFT_ENGINE_MATERIAL_HPP
