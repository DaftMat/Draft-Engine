//
// Created by daftmat on 13/02/20.
//

#ifndef DAFT_ENGINE_LIGHT_HPP
#define DAFT_ENGINE_LIGHT_HPP

#include <opengldemo.h>

enum LightType {
    POINT_LIGHT,
    SPOT_LIGHT,
    DIR_LIGHT
};

union LightParam {
    struct point_param {
        glm::vec3 position;
        GLfloat constant;
        GLfloat linear;
        GLfloat quadratic;
    }pointlight;
    struct dir_param {
        glm::vec3 direction;
    }dirlight;
};

class Light {
public:
    Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);

    Light(const Light &) = delete;
    Light(Light &&) = delete;
    Light &operator=(const Light &) = delete;
    Light &&operator=(Light &&) = delete;

    virtual LightType getType() const = 0;
    virtual LightParam getParams() const = 0;
    virtual void editLight(const LightParam &params) = 0;

    const glm::vec3 & ambient() const { return m_ambient; }
    glm::vec3 & ambient() { return m_ambient; }
    const glm::vec3 & diffuse() const { return m_diffuse; }
    glm::vec3 & diffuse() { return m_diffuse; }
    const glm::vec3 & specular() const { return m_specular; }
    glm::vec3 & specular() { return m_specular; }

private:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
};


#endif //DAFT_ENGINE_LIGHT_HPP
