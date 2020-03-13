//
// Created by daftmat on 14/02/20.
//

#ifndef DAFT_ENGINE_DIRLIGHT_HPP
#define DAFT_ENGINE_DIRLIGHT_HPP

#include "Light.hpp"

/** Emits in one direction, from everywhere. */
class DirLight : public Light
{
  public:
    /** Constructor.
     *
     * @param dir - direction of the directional light.
     * @param color - color emitted by the light.
     */
    explicit DirLight( const glm::vec3& dir   = glm::vec3( 0.f, -1.0f, 0.f ),
                       const glm::vec3& color = glm::vec3( 0.8f, 0.8f, 0.8f ) ) :
        Light( color ),
        m_direction{dir},
        m_basedirection{dir} {
        setupModel();
    }

    /** directional light's type getter.
     * c.f. LightType
     * @return DIR_LIGHT
     */
    LightType getType() const override { return DIR_LIGHT; }

    /** directional light's settings getter.
     * c.f. LightParam
     * @return directional light settings.
     */
    LightParam getParams() const override;

    /** directional light's settings setter.
     * c.f. LightParam
     * @param params - new settings of the directionnal light.
     */
    void editLight( const LightParam& params ) override;

    /** Update directional light.
     * update the rotation of the direction of the light.
     */
    void update() override;

  private:
    void setupModel() override;

    glm::vec3 m_direction;
    glm::vec3 m_basedirection;
};

#endif // DAFT_ENGINE_DIRLIGHT_HPP
