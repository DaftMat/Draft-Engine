/**
 * Created by mathis on 05/03/2020.
 */

#ifndef DAFT_ENGINE_CUBE_HPP
#define DAFT_ENGINE_CUBE_HPP


#include <src/Engine/Geometry/Model.hpp>

class Cube : public Model {
public:
    explicit Cube(GLuint resolution = 2) : m_resolution { resolution } { setupCube(); }

    void reset() override { setupCube(); }

    ModelType getType() const override { return CUBE; }
    std::string getTypeAsString() const override { return "Cube"; }
    void editModel(const ModelParam &params) override;
    ModelParam getParams() const override;

private:
    void setupCube();

    GLuint m_resolution;
};


#endif //DAFT_ENGINE_CUBE_HPP
