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
    ModelManager() : m_selectedmodel { 0 },
    m_blackshader { new Shader("shaders/black.vert.glsl", "shaders/black.frag.glsl") },
    m_yellowshader { new Shader("shaders/yellow.vert.glsl", "shaders/yellow.frag.glsl") },
    m_wireframe { true } {}

    ~ModelManager() { m_models.clear(); m_blackshader.reset(); }

    ModelManager(const ModelManager &) = delete;
    ModelManager(ModelManager &&) = delete;
    ModelManager & operator=(const ModelManager &) = delete;
    ModelManager && operator=(ModelManager &&) = delete;

    void draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection);


    //void addPointLight()

    //void addModel(std::string file)
    void addUVSphere(GLuint meridians = 8, GLuint parallels = 8);
    void addIcoSphere(GLuint subdivisions = 0);

    void translateSelection(const glm::vec3 &transform) { m_models[m_selectedmodel]->translate(transform); }
    void rotateSelection(const glm::vec3 &axis, float angle) { m_models[m_selectedmodel]->rotate(axis, angle); }
    void scaleSelection(const glm::vec3 &transform) { m_models[m_selectedmodel]->scale(transform); }

    bool keyboard(unsigned char key);
    void switch_selection();

    void toggledrawmode() { m_wireframe = !m_wireframe; }

private:
    /// User-Interactions utils functions
    ModelParam add_uvsphere_params(GLuint meridians = 1, GLuint parallels = 1);
    ModelParam sub_uvsphere_params(GLuint meridians = 1, GLuint parallels = 1);
    ModelParam add_icosphere_params(GLuint subdivisions = 1);
    ModelParam sub_icosphere_params(GLuint subdivisions = 1);

    std::vector<std::unique_ptr<Model>> m_models;
    GLuint m_selectedmodel;

    std::vector<std::unique_ptr<Light>> m_lights;

    std::unique_ptr<Shader> m_blackshader;
    std::unique_ptr<Shader> m_yellowshader;

    /// Reset utils
    std::set<GLuint> m_toReset;
    bool m_wireframe;
};


#endif //DAFT_ENGINE_MODELMANAGER_HPP
