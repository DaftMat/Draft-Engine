//
// Created by daftmat on 14/02/20.
//

#ifndef DAFT_ENGINE_DIRLIGHT_HPP
#define DAFT_ENGINE_DIRLIGHT_HPP

#include "Light.hpp"

class DirLight : public Light {
public:
    DirLight(const glm::vec3 &dir=glm::vec3(-0.2f, -1.0f, -0.3f),
             const glm::vec3 &ambient=glm::vec3(0.05f, 0.05f, 0.05f),
             const glm::vec3 &diffuse=glm::vec3(0.6f, 0.6f, 0.6f),
             const glm::vec3 &specular=glm::vec3(0.8f, 0.8f, 0.8f))
             : Light(ambient, diffuse, specular), m_direction { dir }
    {}

    LightType getType() const override { return DIR_LIGHT; }

    LightParam getParams() const override;
    void editLight(const LightParam &params) override { m_direction = params.dirlight.direction; }

private:
    glm::vec3 m_direction;
};


#endif //DAFT_ENGINE_DIRLIGHT_HPP