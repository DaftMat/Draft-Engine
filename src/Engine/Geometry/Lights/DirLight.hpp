//
// Created by daftmat on 14/02/20.
//

#ifndef DAFT_ENGINE_DIRLIGHT_HPP
#define DAFT_ENGINE_DIRLIGHT_HPP

#include "Light.hpp"

class DirLight : public Light {
public:
    explicit DirLight(const glm::vec3 &dir=glm::vec3(0.f, -1.0f, 0.f),
             const glm::vec3 &ambient=glm::vec3(0.05f, 0.05f, 0.05f),
             const glm::vec3 &diffuse=glm::vec3(0.6f, 0.6f, 0.6f),
             const glm::vec3 &specular=glm::vec3(0.8f, 0.8f, 0.8f))
             : Light(ambient, diffuse, specular), m_direction { dir }, m_basedirection { dir }
    { setupModel(); }

    LightType getType() const override { return DIR_LIGHT; }

    LightParam getParams() const override;
    void editLight(const LightParam &params) override;

    void update() override;

private:
    void setupModel() override;

    glm::vec3 m_direction;
    glm::vec3 m_basedirection;
};


#endif //DAFT_ENGINE_DIRLIGHT_HPP
