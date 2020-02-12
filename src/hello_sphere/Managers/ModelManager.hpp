/**
 * Created by mathis on 12/02/2020.
 */

#ifndef DAFT_ENGINE_MODELMANAGER_HPP
#define DAFT_ENGINE_MODELMANAGER_HPP

#include <opengldemo.h>
#include <hello_sphere/Geometry/Model.hpp>
#include <vector>
#include <set>
#include <memory>

class ModelManager {
public:
    ModelManager() : m_selectedmodel { 0 } {}
    ~ModelManager() { m_models.clear(); }

    ModelManager(const ModelManager &) = delete;
    ModelManager(ModelManager &&) = delete;
    ModelManager & operator=(const ModelManager &) = delete;
    ModelManager && operator=(ModelManager &&) = delete;

    void draw(const Shader &shader);

    //void addModel(std::string file)
    void addUVSphere(GLuint meridians = 8, GLuint parallels = 8);
    void addIcoSphere(GLuint subdivisions = 0);

    void translateSelection(const glm::vec3 &transform) { m_models[m_selectedmodel]->translate(transform); }

    bool keyboard(unsigned char key);
    void switch_selection();

private:
    /// User-Interactions utils functions
    ModelParam add_uvsphere_params(GLuint meridians = 1, GLuint parallels = 1);
    ModelParam add_icosphere_params(GLuint subdivisions = 1);
    ModelParam sub_uvsphere_params(GLuint meridians = 1, GLuint parallels = 1);
    ModelParam sub_icosphere_params(GLuint subdivisions = 1);

    std::vector<std::unique_ptr<Model>> m_models;
    GLuint m_selectedmodel;

    /// Reset utils
    std::set<GLuint> m_toReset;
};


#endif //DAFT_ENGINE_MODELMANAGER_HPP
