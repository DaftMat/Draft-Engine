/**
 * Created by mathis on 12/02/2020.
 */

#ifndef DAFT_ENGINE_MODELMANAGER_HPP
#define DAFT_ENGINE_MODELMANAGER_HPP

#include <opengl_stuff.h>
#include <Engine/Geometry/Model.hpp>
#include <vector>
#include <set>
#include <memory>
#include <src/Engine/Geometry/Lights/PointLight.hpp>
#include <src/Engine/Physics/Ray.hpp>
#include <src/Engine/Managers/Gizmos/TranslateGizmo.hpp>

class ModelManager {
public:
    ModelManager() : m_selectedmodel { -1 },
    m_colorshader { new Shader("shaders/color.vert.glsl", "shaders/color.frag.glsl") },
    m_wireframe { true },
    m_gizmo { new TranslateGizmo(Utils::Transform::Identity()) } { makeGrid(50); makeUnitArrows(); }

    ~ModelManager() { m_models.clear(); m_lights.clear(); m_colorshader.reset(); }

    ModelManager(const ModelManager &) = delete;
    ModelManager(ModelManager &&) = delete;
    ModelManager & operator=(const ModelManager &) = delete;
    ModelManager && operator=(ModelManager &&) = delete;

    void draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &viewPos);

    void addPointLight(
            const glm::vec3 &pos=glm::vec3(0.f, 0.f, 0.f),
            GLfloat constant = 1.f,
            GLfloat linear = 0.09f,
            GLfloat quadratic = 0.032f,
            const glm::vec3 &ambient=glm::vec3(0.05f, 0.05f, 0.05f),
            const glm::vec3 &diffuse=glm::vec3(0.8f, 0.8f, 0.8f),
            const glm::vec3 &specular=glm::vec3(1.f, 1.f, 1.f));
    void addDirLight(
            const glm::vec3 &dir=glm::vec3(-0.2f, -1.0f, -0.3f),
            const glm::vec3 &ambient=glm::vec3(0.05f, 0.05f, 0.05f),
            const glm::vec3 &diffuse=glm::vec3(0.6f, 0.6f, 0.6f),
            const glm::vec3 &specular=glm::vec3(0.8f, 0.8f, 0.8f));
    void addSpotLight(
            const glm::vec3 &pos=glm::vec3(0.0f, 0.0f, 0.0f),
            const glm::vec3 &dir=glm::vec3(0.0f, -1.f, 0.0f),
            float innerCutoff=20.5f,
            float outerCutoff=30.0f,
            float constant=1.0f,
            float linear=0.09f,
            float quadratic=0.032f,
            const glm::vec3 &ambient=glm::vec3(0.05f, 0.05f, 0.05f),
            const glm::vec3 &diffuse=glm::vec3(0.8f, 0.8f, 0.8f),
            const glm::vec3 &specular=glm::vec3(1.f, 1.0f, 1.0f));

    //void addModel(std::string file)
    void addUVSphere(GLuint meridians = 32, GLuint parallels = 16);
    void addIcoSphere(GLuint subdivisions = 3);
    void addCubeSphere(GLuint resolution = 16);

    void deleteModel();

    bool keyboard(unsigned char key);
    void switch_selection();

    void toggledrawmode() { m_wireframe = !m_wireframe; }

    Model * getSelectedObject() { if (m_selectedmodel == -1) return nullptr; else return m_models[m_selectedmodel].get(); }
    int getSelectedIndex() const { return m_selectedmodel; }
    void setSelectedIndex(GLuint index) { m_selectedmodel = glm::max(index, (GLuint)(m_models.size() - 1)); }
    unsigned long getSize() const { return m_models.size(); }

    void setUVSphereParams(GLuint meridians, GLuint parallels);
    void setIcoSphereParams(GLuint subdivisions);
    void setCubeSphereParams(GLuint resolution);

    bool mouse_click(const Ray &ray, float xpos, float ypos);
    void mouse_move(float xpos, float ypos, const glm::mat4 &projection, const glm::mat4 &view);
    void mouserelease();

private:
    void drawGrid(const glm::mat4 &projection, const glm::mat4 &view);
    void makeGrid(int size);
    void makeUnitArrows();

    void updateGizmo(const glm::vec3 &viewPos);

    std::vector<glm::vec3> dirs() {
        return {
                { 1.f, 0.f, 0.f },
                { 0.f, 1.f, 0.f },
                { 0.f, 0.f, 1.f }
        };
    }

    std::unique_ptr<Mesh> m_grid;
    std::vector<std::unique_ptr<Mesh>> m_unitarrows;

    std::vector<std::unique_ptr<Model>> m_models;
    int m_selectedmodel;

    std::vector<std::unique_ptr<Light>> m_lights;

    std::unique_ptr<Shader> m_colorshader;

    /// Reset utils
    std::set<GLuint> m_toReset;
    bool m_wireframe;

    std::unique_ptr<Gizmo> m_gizmo;
};


#endif //DAFT_ENGINE_MODELMANAGER_HPP
