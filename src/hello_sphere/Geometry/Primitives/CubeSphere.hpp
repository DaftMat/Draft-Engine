/**
 * Created by mathis on 13/02/2020.
 */

#ifndef DAFT_ENGINE_CUBESPHERE_HPP
#define DAFT_ENGINE_CUBESPHERE_HPP

#include <src/hello_sphere/Geometry/Model.hpp>

class CubeSphere : public Model {
public:
    explicit CubeSphere(GLuint resolution = 8) : m_resolution { resolution } { setupCubeSphere(); }

    void reset() override { setupCubeSphere(); }

    ModelType getType() const override { return CUBE_SPHERE; }
    void editModel(const ModelParam &params) override;
    ModelParam getParams() const override;

private:
    void setupCubeSphere();

    GLuint m_resolution;
};


#endif //DAFT_ENGINE_CUBESPHERE_HPP
