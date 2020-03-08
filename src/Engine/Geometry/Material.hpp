/**
 * Created by mathis on 08/03/2020.
 */

#ifndef DAFT_ENGINE_MATERIAL_HPP
#define DAFT_ENGINE_MATERIAL_HPP

#include <opengl_stuff.h>

class Material {
public:
    Material() : m_albedo { 0.5f, 0.0f, 0.0f }, m_metalness { 0.8f }, m_roughness { 0.2f }, m_ao { 0.5f } {}
    Material(const glm::vec3 &albedo, float metalness, float roughness, float ao) :
    m_albedo { albedo },
    m_metalness { metalness },
    m_roughness { roughness },
    m_ao { ao } {}

    glm::vec3 &albedo() { return m_albedo; }
    const glm::vec3 &albedo() const { return m_albedo; }
    float &metalness() { return m_metalness; }
    float metalness() const { return m_metalness; }
    float &roughness() { return m_roughness; }
    float roughness() const { return m_roughness; }
    float &ambientOcclusion() { return m_ao; }
    float ambientOcclusion() const { return m_ao; }

private:
    glm::vec3 m_albedo;
    float m_metalness;
    float m_roughness;
    float m_ao;
};


#endif //DAFT_ENGINE_MATERIAL_HPP
