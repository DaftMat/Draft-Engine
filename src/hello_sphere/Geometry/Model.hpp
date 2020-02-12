//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_MODEL_HPP
#define DAFT_ENGINE_MODEL_HPP


#include "Mesh.hpp"
#include "src/hello_sphere/Shader.hpp"

union ModelParam {
    struct uvs_param {
        GLuint meridians;
        GLuint parallels;
    }uv_sphere;
    struct icos_param {
        GLuint subdivisions;
    }ico_sphere;
    struct cubs_param {
        GLuint resolution;
    }cube_sphere;
};

enum ModelType {
    MODEL,
    UV_SPHERE,
    ICO_SPHERE,
    CUBE_SPHERE
};

class Model {
public:
    Model() = default;
    ~Model() { m_meshes.clear(); }

    Model(const Model &) = delete;
    Model & operator=(const Model &) = delete;

    void draw(const Shader &shader) const;

    void transform(const glm::mat4 &op) { m_model = op; }
    glm::mat4 model() { return m_model; }

    void translate(const glm::vec3 & transform) { m_model = glm::translate(m_model, transform); }

    virtual void reset() {}

    virtual ModelType getType() const { return MODEL; }
    virtual void editModel(const ModelParam &params) = 0;
    virtual ModelParam getParams() const = 0;

protected:
    std::vector<Mesh> m_meshes;

private:
    glm::mat4 m_model;
};


#endif //DAFT_ENGINE_MODEL_HPP
