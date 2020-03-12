//
// Created by mathis on 13/02/2020.
//

#ifndef DAFT_ENGINE_POINTLIGHT_HPP
#define DAFT_ENGINE_POINTLIGHT_HPP

#include "Light.hpp"

/** Point Light.
 *  A Light that emits in all direction from one point.
 */
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
        Light( color ), m_position{pos}, m_intensity{intensity} {
        setupModel();
    }

    /** PointLight's type getter. */
    LightType getType() const override { return POINT_LIGHT; }
    /** PointLight's settings getter. */
    LightParam getParams() const override;
    /** PointLight's settings setter. */
    void editLight( const LightParam& params ) override;

    void update() override { m_position = m_model->getPosition(); }

  private:
    void setupModel() override;

    glm::vec3 m_position;

    GLfloat m_intensity;
};

#endif // DAFT_ENGINE_POINTLIGHT_HPP
