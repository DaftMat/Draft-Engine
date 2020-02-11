//
// Created by daftmat on 10/02/20.
//

#ifndef DAFT_ENGINE_MODEL_HPP
#define DAFT_ENGINE_MODEL_HPP


#include "Mesh.hpp"
#include "src/hello_sphere/Shader.hpp"

class Model {
public:
    Model() = default;
    ~Model() { m_meshes.clear(); }

    Model(const Model &) = delete;
    Model & operator=(const Model &) = delete;

    void draw(const Shader &shader) const;

    void transform(const glm::mat4 &op) { m_model = op; }
    glm::mat4 model() { return m_model; }

    virtual void reset() {}

protected:
    /// Primitives functions
    virtual void setupUvSphere() {}
    virtual void setupIcoSphere() {}
    virtual void setupCubeSphere() {}
    virtual void setupPlane() {}
    virtual void setupCylinder() {}
    virtual void setupPyramid() {}
    virtual void setupCube() {}
    virtual void setupTore() {}

    std::vector<Mesh> m_meshes;

private:
    glm::mat4 m_model;
};


#endif //DAFT_ENGINE_MODEL_HPP
