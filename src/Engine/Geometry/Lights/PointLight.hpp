/**
 * Created by mathis on 13/02/2020.
 */

#ifndef DAFT_ENGINE_POINTLIGHT_HPP
#define DAFT_ENGINE_POINTLIGHT_HPP

#include "Light.hpp"

class PointLight : public Light {
public:
    explicit PointLight(const glm::vec3 &pos=glm::vec3(0.f, 0.f, 0.f),
            GLfloat intensity = 1.f,
            const glm::vec3 &color=glm::vec3(0.8f, 0.8f, 0.8f))
            :   Light(color),
                m_position { pos },
                m_intensity { intensity } { setupModel(); }

    LightType getType() const override { return POINT_LIGHT; }

    LightParam getParams() const override;
    void editLight(const LightParam &params) override;

    void update() override { m_position = m_model->getPosition(); }

private:
    void setupModel() override;

    glm::vec3 m_position;

    GLfloat m_intensity;
};


#endif //DAFT_ENGINE_POINTLIGHT_HPP
