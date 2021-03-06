//
// Created by daftmat on 14/02/20.
//

#ifndef DAFT_ENGINE_SPOTLIGHT_HPP
#define DAFT_ENGINE_SPOTLIGHT_HPP

#include "Light.hpp"

/** A Light that emits within a directed angle from a point. */
class SpotLight : public Light
{
  public:
    /** Constructor.
     *
     * @param pos - position of the SpotLight.
     * @param dir - direction of the SpotLight.
     * @param innerCutoff - inner angle of the SpotLight.
     * @param outerCutoff - outer angle of the SpotLight.
     * @param intensity - intensity of the SpotLight.
     * @param color - color emitted by the SpotLight.
     */
    SpotLight( const glm::vec3& pos   = glm::vec3( 0.0f, 0.0f, 0.0f ),
               const glm::vec3& dir   = glm::vec3( 0.0f, -1.f, 0.0f ),
               float innerCutoff      = 12.5f,
               float outerCutoff      = 15.0f,
               float intensity        = 1.0f,
               const glm::vec3& color = glm::vec3( 0.8f, 0.8f, 0.8f ) ) :
        Light( color ),
        m_position{pos},
        m_direction{dir},
        m_baseDirection{dir},
        m_innerCutoff{innerCutoff},
        m_outerCutoff{outerCutoff},
        m_intensity{intensity} {
        setupModel();
    }

    /** spot light's type getter.
     * c.f. LightType
     * @return SPOT_LIGHT
     */
    LightType getType() const override { return SPOT_LIGHT; }

    /** spot light's settings getter.
     * c.f. LightParam
     * @return spot light settings.
     */
    LightParam getParams() const override;

    /** spot light's settings setter.
     * c.f. LightParam
     * @param params - new spot light settings.
     */
    void editLight( const LightParam& params ) override;

    /** update the point light.
     * update the point light position and rotation using its model
     */
    void update() override;

  private:
    void setupModel() override;

    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_baseDirection;
    float m_innerCutoff;
    float m_outerCutoff;
    float m_intensity;
};

#endif // DAFT_ENGINE_SPOTLIGHT_HPP
