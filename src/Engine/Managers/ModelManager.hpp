//
// Created by mathis on 12/02/2020.
//

#ifndef DAFT_ENGINE_MODELMANAGER_HPP
#define DAFT_ENGINE_MODELMANAGER_HPP

#include <Engine/Geometry/Lights/DirLight.hpp>
#include <Engine/Geometry/Model.hpp>
#include <memory>
#include <opengl_stuff.h>
#include <set>
#include <src/Engine/Geometry/Lights/PointLight.hpp>
#include <src/Engine/Managers/Gizmos/ScaleGizmo.hpp>
#include <src/Engine/Managers/Gizmos/TranslateGizmo.hpp>
#include <src/Engine/Physics/Ray.hpp>
#include <vector>
#include <src/Engine/Rendering/Raytracer/Raytracer.hpp>

/**
 *  Set of Model and Light to be drawn.
 *  Class used in Engine.
 */
class ModelManager
{
  public:
    /** Constructor.
     * creates the model manager for the engine.
     */
    ModelManager(int width, int height) :
        m_selectedmodel{-1},
        m_selectedlight{-1},
        m_editionlight{new DirLight},
        m_colorshader{new Shader( "shaders/color.vert.glsl", "shaders/color.frag.glsl" )},
        m_particleshader{new Shader( "shaders/particle.vert.glsl", "shaders/particle.frag.glsl" )},
        m_wireframe{true},
        m_edition{true},
        m_gizmo{new TranslateGizmo( Utils::Transform::Identity() )},
        m_raytracer { new Raytracer(width, height) }{
        makeGrid( 50 );
        makeUnitArrows();
    }

    ~ModelManager() {
        m_models.clear();
        m_lights.clear();
        m_editionlight.reset();
        m_colorshader.reset();
        m_raytracer.reset();
    }

    ModelManager( const ModelManager& ) = delete;
    ModelManager( ModelManager&& )      = delete;
    ModelManager& operator=( const ModelManager& ) = delete;
    ModelManager&& operator=( ModelManager&& ) = delete;

    /** Draw all models and lights of the scene.
     *
     * @param shader - Shader to be used for drawing.
     * @param view - view matrix of the Camera.
     * @param projection - projection matrix of the Camera.
     * @param viewPos - position of the Camera.
     * @param viewDir - view direction (look at) of the Camera.
     */
    void draw( float dt, Shader& shader,
               const glm::mat4& view,
               const glm::mat4& projection,
               const glm::vec3& viewPos,
               const glm::vec3& viewDir );

    /** Add a Light with default settings to the scene.
     *
     * @param type - type of the Light to be added.
     */
    void addLight( Light::LightType type );
    // void addModel(std::string file)
    /** Add a Model with default settings to the scene.
     *
     * @param type - type of the Model to be added.
     */
    void addObject( Model::ModelType type );

    /** Delete the selected Model or Light.
     *  Does nothing is nothing is selected.
     */
    void deleteModel();

    /** keyboard key press event handler.
     *
     * @param key - key pressed
     * @return true if the key triggered an action.
     */
    bool keyboard( unsigned char key );

    /** Toggles edges drawing on models.
     * In edition mode only.
     */
    void toggledrawmode() { m_wireframe = !m_wireframe; }

    /** Selected Model ptr getter.
     *
     * @return nullptr if no Model selected.
     */
    Model* getSelectedObject();

    /** Selected Model 's index getter.
     *
     * @return -1 if no Model selected.
     */
    int getSelectedIndex() const { return m_selectedmodel; }

    /** Selected model's index setter.
     *
     * @param index - new index to select.
     */
    void setSelectedIndex( GLuint index );

    /** Number of models in scene getter.
     *
     * @return number of models in the scene.
     */
    unsigned long getSize() const { return m_models.size(); }

    /** Selected object settings setter.
     * Does nothing if no object selected.
     * @param params - new settings for the selected model.
     */
    void setObjectParams( const Model::ModelParam& params );

    /** Selected Light ptr getter.
     *
     * @return nullptr if no Light selected.
     */
    Light* getSelectedLight();

    /** Selected Light index getter.
     *
     * @return -1 if no Light selected.
     */
    int getSelectedLightIndex() const { return m_selectedlight; }

    /** Selected Light index setter.
     *
     * @param index - new index for the selected model.
     */
    void setSelectedLight( GLuint index );

    /** Number of Light in scene getter.
     *
     * @return number of lights in the scene.
     */
    unsigned long getNumLights() const { return m_lights.size(); }

    /** mouse click event handler.
     *
     * @param ray - calculated from Ray "mouse click" constructor.
     * @param xpos - x position of mouse.
     * @param ypos - y position of mouse.
     * @return true if an action has been triggered.
     */
    bool mouse_click( const Ray& ray, float xpos, float ypos );

    /** mouse move event handler.
     *
     * @param xpos - x position of the mouse.
     * @param ypos - y position of the mouse.
     * @param projection - projection matrix of the Camera.
     * @param view - view matrix of the Camera.
     */
    void mouse_move( float xpos, float ypos, const glm::mat4& projection, const glm::mat4& view );

    /** mouse release event.
     * used in gizmo selection.
     */
    void mouserelease();

    /** Change Gizmo 's transformation style
     *  Can be translation of scale gizmo
     * @param type - new type of the gizmo
     */
    void switchGizmo( Gizmo::GizmoType type ) { m_gizmoType = type; };

    /** Toggle Edition Mode.
     *  Edition Mode draws scene with stuff for edition, if disabled,
     *  nothing more than the scene is rendered, and the sky turn black.
     */
    void toggleEditionMode() { m_edition = !m_edition; }

    /** Render the scene using Raytracer.
     *
     * @param path - path of the output picture.
     */
    void raytrace(const std::string &path, int width, int height, const glm::mat4 & projection, const glm::mat4 & view);

  private:
    void drawGrid( const glm::mat4& projection, const glm::mat4& view );
    void makeGrid( int size );
    void makeUnitArrows();

    void updateGizmo( const glm::vec3& viewPos );
    void switchGizmo();

    std::vector<glm::vec3> dirs() { return {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}}; }

    std::unique_ptr<Mesh> m_grid;
    std::vector<std::unique_ptr<Mesh>> m_unitarrows;

    std::vector<std::unique_ptr<Model>> m_models;
    int m_selectedmodel;

    std::vector<std::unique_ptr<Light>> m_lights;
    int m_selectedlight;
    std::unique_ptr<Light> m_editionlight;

    std::unique_ptr<Shader> m_colorshader;
    std::unique_ptr<Shader> m_particleshader;

    /// Reset utils
    std::set<GLuint> m_toReset;
    bool m_wireframe;
    bool m_edition;

    std::unique_ptr<Gizmo> m_gizmo;
    Gizmo::GizmoType m_gizmoType{Gizmo::TRANSLATE};

    std::unique_ptr<Raytracer> m_raytracer;
};

#endif // DAFT_ENGINE_MODELMANAGER_HPP
