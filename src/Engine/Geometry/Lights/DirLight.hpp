//
// Created by daftmat on 14/02/20.
//

#ifndef DAFT_ENGINE_DIRLIGHT_HPP
#define DAFT_ENGINE_DIRLIGHT_HPP

#include "Light.hpp"

/** Directional Light.
 *  Emits in one direction, from everywhere.
 */
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

    /** DirLight's type getter. */
    LightType getType() const override { return DIR_LIGHT; }
    /** DirLight's settings getter. */
    LightParam getParams() const override;
    /** DirLight's settings setter. */
    void editLight( const LightParam& params ) override;

    void update() override;

  private:
    void setupModel() override;

    glm::vec3 m_direction;
    glm::vec3 m_basedirection;
};

#endif // DAFT_ENGINE_DIRLIGHT_HPP
