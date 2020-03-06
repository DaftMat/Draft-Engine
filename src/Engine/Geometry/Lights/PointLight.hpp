/**
 * Created by mathis on 13/02/2020.
 */

#ifndef DAFT_ENGINE_POINTLIGHT_HPP
#define DAFT_ENGINE_POINTLIGHT_HPP

#include "Light.hpp"

class PointLight : public Light {
public:
    explicit PointLight(const glm::vec3 &pos=glm::vec3(0.f, 0.f, 0.f),
            GLfloat constant = 1.f,
            GLfloat linear = 0.09f,
            GLfloat quadratic = 0.032f,
            const glm::vec3 &ambient=glm::vec3(0.05f, 0.05f, 0.05f),
            const glm::vec3 &diffuse=glm::vec3(0.8f, 0.8f, 0.8f),
            const glm::vec3 &specular=glm::vec3(1.f, 1.f, 1.f))
            :   Light(ambient, diffuse, specular),
                m_position { pos },
                m_constant { constant },
                m_linear { linear },
                m_quadratic { quadratic } { }

    LightType getType() const override { return POINT_LIGHT; }

    LightParam getParams() const override;
    void editLight(const LightParam &params) override;

private:

    glm::vec3 m_position;

    GLfloat m_constant;
    GLfloat m_linear;
    GLfloat m_quadratic;
};


#endif //DAFT_ENGINE_POINTLIGHT_HPP
