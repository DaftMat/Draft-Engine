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
#include <Engine/Geometry/Lights/DirLight.hpp>
#include <src/Engine/Physics/Ray.hpp>
#include <src/Engine/Managers/Gizmos/TranslateGizmo.hpp>
#include <src/Engine/Managers/Gizmos/ScaleGizmo.hpp>

class ModelManager {
public:
    ModelManager() : m_selectedmodel { -1 }, m_selectedlight { -1 },
    m_editionlight { new DirLight },
    m_colorshader { new Shader("shaders/color.vert.glsl", "shaders/color.frag.glsl") },
    m_wireframe { true },
    m_edition { true },
    m_gizmo { new TranslateGizmo(Utils::Transform::Identity()) } { makeGrid(50); makeUnitArrows(); }

    ~ModelManager() { m_models.clear(); m_lights.clear(); m_editionlight.reset(); m_colorshader.reset(); }

    ModelManager(const ModelManager &) = delete;
    ModelManager(ModelManager &&) = delete;
    ModelManager & operator=(const ModelManager &) = delete;
    ModelManager && operator=(ModelManager &&) = delete;

    void draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &viewPos, const glm::vec3 &viewDir);

    void addLight(LightType type);
    //void addModel(std::string file)
    void addObject(ModelType type);

    void deleteModel();

    bool keyboard(unsigned char key);

    void toggledrawmode() { m_wireframe = !m_wireframe; }

    Model * getSelectedObject() { if (m_selectedmodel == -1) return nullptr; else return m_models[m_selectedmodel].get(); }
    int getSelectedIndex() const { return m_selectedmodel; }
    void setSelectedIndex(GLuint index) { m_selectedmodel = glm::max(index, (GLuint)(m_models.size() - 1)); m_selectedlight = -1; }
    unsigned long getSize() const { return m_models.size(); }
    void setObjectParams(const ModelParam &params);

    Light * getSelectedLight() { if (m_selectedlight == -1) return nullptr; else return m_lights[m_selectedlight].get(); }
    int getSelectedLightIndex() const { return m_selectedlight; }
    void setSelectedLight(GLuint index) { m_selectedlight = glm::max(index, GLuint(m_models.size() - 1)); m_selectedmodel = -1; }
    unsigned long getNumLights() const { return m_lights.size(); }
    void setLightParams(const LightParam &params, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);

    bool mouse_click(const Ray &ray, float xpos, float ypos);
    void mouse_move(float xpos, float ypos, const glm::mat4 &projection, const glm::mat4 &view);
    void mouserelease();

    void switchGizmo(GizmoType type) { m_gizmoType = type; };

    void toggleEditionMode() { m_edition = !m_edition; }

private:
    void drawGrid(const glm::mat4 &projection, const glm::mat4 &view);
    void makeGrid(int size);
    void makeUnitArrows();

    void updateGizmo(const glm::vec3 &viewPos);
    void switchGizmo();

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
    int m_selectedlight;
    std::unique_ptr<Light> m_editionlight;

    std::unique_ptr<Shader> m_colorshader;

    /// Reset utils
    std::set<GLuint> m_toReset;
    bool m_wireframe;
    bool m_edition;

    std::unique_ptr<Gizmo> m_gizmo;
    GizmoType m_gizmoType {TRANSLATE};
};


#endif //DAFT_ENGINE_MODELMANAGER_HPP
