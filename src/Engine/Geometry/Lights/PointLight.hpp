//
// Created by mathis on 13/02/2020.
//

#ifndef DAFT_ENGINE_POINTLIGHT_HPP
#define DAFT_ENGINE_POINTLIGHT_HPP

#include "Light.hpp"

/** A Light that emits in all direction from one point. */
class PointLight : public Light
{
  public:
    /** Constructor.
     *
     * @param pos - position of the PointLight.
     * @param intensity - intensity of the PointLight.
     * @param color - color emitted by the PointLight.
     */
    explicit PointLight( const glm::vec3& pos   = glm::vec3( 0.f, 0.f, 0.f ),
                         GLfloat intensity      = 1.f,
                         const glm::vec3& color = glm::vec3( 0.8f, 0.8f, 0.8f ) ) :
        Light( color ),
        m_position{pos},
        m_intensity{intensity} {
        setupModel();
    }

    /** point light's type getter.
     * c.f. LightType
     * @return POINT_LIGHT
     */
    LightType getType() const override { return POINT_LIGHT; }

    /** point light's settings getter.
     * c.f. LightParam
     * @return point light settings.
     */
    LightParam getParams() const override;

    /** point light's settings setter.
     * c.f. LightParam
     * @param params - new point light's settings.
     */
    void editLight( const LightParam& params ) override;

    /** update point light.
     * update position of the light using its model.
     */
    void update() override { m_position = m_model->getPosition(); }

  private:
    void setupModel() override;

    glm::vec3 m_position;

    GLfloat m_intensity;
};

#endif // DAFT_ENGINE_POINTLIGHT_HPP
