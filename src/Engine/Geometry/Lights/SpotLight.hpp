//
// Created by daftmat on 14/02/20.
//

#ifndef DAFT_ENGINE_SPOTLIGHT_HPP
#define DAFT_ENGINE_SPOTLIGHT_HPP

#include "Light.hpp"

class SpotLight : public Light{
public:
    SpotLight(
            const glm::vec3 &pos=glm::vec3(0.0f, 0.0f, 0.0f),
            const glm::vec3 &dir=glm::vec3(0.0f, -1.f, 0.0f),
            float innerCutoff=12.5f,
            float outerCutoff=15.0f,
            float constant=1.0f,
            float linear=0.09f,
            float quadratic=0.032f,
            const glm::vec3 &ambient=glm::vec3(0.05f, 0.05f, 0.05f),
            const glm::vec3 &diffuse=glm::vec3(0.8f, 0.8f, 0.8f),
            const glm::vec3 &specular=glm::vec3(1.f, 1.0f, 1.0f)
            ) : Light(ambient, diffuse, specular),
                m_position { pos },
                m_direction { dir},
                m_innerCutoff { innerCutoff },
                m_outerCutoff { outerCutoff },
                m_constant { constant },
                m_linear { linear },
                m_quadratic { quadratic }
    {}

    LightType getType() const override { return SPOT_LIGHT; }

    LightParam getParams() const override;
    void editLight(const LightParam &params) override;

    void update() override;

private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    float m_innerCutoff;
    float m_outerCutoff;
    float m_constant;
    float m_linear;
    float m_quadratic;
};


#endif //DAFT_ENGINE_SPOTLIGHT_HPP
