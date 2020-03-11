//
// Created by daftmat on 13/02/20.
//

#ifndef DAFT_ENGINE_LIGHT_HPP
#define DAFT_ENGINE_LIGHT_HPP

//#include <opengl_stuff.h>
#include <src/Engine/Geometry/Model.hpp>

enum LightType {
    POINT_LIGHT,
    SPOT_LIGHT,
    DIR_LIGHT
};

union LightParam {
    struct point_param {
        glm::vec3 position;
        GLfloat intensity;
    }pointlight;
    struct dir_param {
        glm::vec3 direction;
        glm::vec3 rotations;
    }dirlight;
    struct spot_param {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 rotations; //for direction transformation
        GLfloat innerCutoff;
        GLfloat outerCutoff;
        GLfloat intensity;
    }spotlight;
};

class Light {
public:
    Light(const glm::vec3 &color);
    virtual ~Light() { m_model.reset(); }

    Light(const Light &) = delete;
    Light(Light &&) = delete;
    Light &operator=(const Light &) = delete;
    Light &&operator=(Light &&) = delete;

    virtual LightType getType() const = 0;
    virtual LightParam getParams() const = 0;
    virtual void editLight(const LightParam &params) = 0;

    const glm::vec3 & color() const { return m_color; }
    glm::vec3 & color() { return m_color; }

    const Model& model() const { return *m_model; }
    Model& model() { return *m_model; }

    virtual void update() {};
    void reset() { setupModel(); }

protected:
    std::unique_ptr<Model> m_model;

private:
    virtual void setupModel() {};

    glm::vec3 m_color;
};


#endif //DAFT_ENGINE_LIGHT_HPP
