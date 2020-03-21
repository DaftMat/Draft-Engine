//
// Created by mathis on 08/03/2020.
//

#ifndef DAFT_ENGINE_MATERIAL_HPP
#define DAFT_ENGINE_MATERIAL_HPP

#include <opengl_stuff.h>

/** Only for PBR shader. */
class Material
{
  public:
    /** Default Constructor.
     *  set material settings to default values.
     */
    Material() :
        m_albedo{0.8f, 0.f, 0.8f},
        m_specular{0.9f, 0.9f, 0.9f},
        m_roughness{0.1f},
        m_ao{1.f},
        m_ior{1.777},
        m_transparency { 0.f } {}

    /** Constructor.
     *
     * @param albedo - albedo color of the Material.
     * @param specular - specular color of the Material.
     * @param roughness - roughness of the Material.
     * @param ao - ambient occlusion of the Material.
     * @param ior - index of refraction of the Material.
     */
    Material( const glm::vec3& albedo,
              const glm::vec3& specular,
              float roughness,
              float ao,
              float ior ) :
        m_albedo{albedo},
        m_specular{specular},
        m_roughness{roughness},
        m_ao{ao},
        m_ior{ior} {}

    /** albedo color ref.
     *
     * @return reference to albedo color of the material.
     */
    glm::vec3& albedo() { return m_albedo; }

    /** albedo color const ref.
     *
     * @return constant reference to albedo color of the material.
     */
    const glm::vec3& albedo() const { return m_albedo; }

    /** specular color ref.
     *
     * @return reference to specular color of the material.
     */
    glm::vec3& specular() { return m_specular; }

    /** specular color const ref.
     *
     * @return constant reference to specular color of the material.
     */
    const glm::vec3& specular() const { return m_specular; }

    /** roughness ref.
     *
     * @return reference to roughness of the material.
     */
    float& roughness() { return m_roughness; }
    /** roughness getter.
     *
     * @return roughness of the material.
     */
    float roughness() const { return m_roughness; }

    /** ambient occlusion ref.
     *
     * @return reference to the AO of the material.
     */
    float& ambientOcclusion() { return m_ao; }

    /** ambient occlusion getter.
     *
     * @return AO of the material.
     */
    float ambientOcclusion() const { return m_ao; }

    /** index of refraction ref.
     *
     * @return reference to the IOR of the material.
     */
    float& IOR() { return m_ior; }

    /** index of refraction getter.
     *
     * @return IOR of the material.
     */
    float IOR() const { return m_ior; }

    /** transparency ref.
     * for Raytracer only.
     * @return reference to the transparency of the material.
     */
    float& transparency() { return m_transparency; }

    /** transparency getter.
     * for Raytracer only.
     * @return
     */
    float transparency() const { return m_transparency; }

  private:
    glm::vec3 m_albedo;
    glm::vec3 m_specular;
    float m_roughness;
    float m_ao;
    float m_ior;
    float m_transparency;
};

#endif // DAFT_ENGINE_MATERIAL_HPP
