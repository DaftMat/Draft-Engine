//
// Created by daftmat on 27/01/20.
//

#ifndef IIG3D_BASETP_HELLOSPHERES_HPP
#define IIG3D_BASETP_HELLOSPHERES_HPP

#include <functional>
#include <memory>

#include <Engine/Shader.hpp>
#include <src/Engine/Cameras/camera.h>
#include <src/Engine/Managers/ModelManager.hpp>

/** Defines the whole drew scene. */
class Engine
{
  public:
    /** Constructor.
     *
     * @param width - width of the OpenGL window.
     * @param height - height of the OpenGL window.
     */
    explicit Engine( int width, int height );
    ~Engine();

    /** resize the OpenGL window.
     *
     * @param width - new OpenGL's width.
     * @param height - new OpenGL's height.
     */
    void resize( int width, int height );

    /** draw the scene.
     *  draw all the models and lights of the ModelManager using selected shader.
     */
    void draw();

    /** mouse click event handler.
     * can start to move camera, select a model or a light or a gizmo to move.
     * @param button - which mouse button has been clicked
     * @param xpos - x position of the mouse on OpenGL screen.
     * @param ypos - y position of the mouse on OpenGL screen.
     */
    void mouseclick( int button, float xpos, float ypos );

    /** mouse release event handler.
     * used to reset gizmo selection to "None".
     */
    void mouserelease();

    /** mouse move event handler.
     * used after a mouse click.
     * can move camera or a gizmo. nothing happen if a model or a light get selected just before.
     * @param xpos - x position of the mouse on the OpenGL screen.
     * @param ypos - y position of the mouse on the OpenGL screen.
     */
    void mousemove( float xpos, float ypos );

    /** keyboard arrows click handler. */
    void keyboardmove( int key, double time );
    /** keyboard keys click handler. */
    bool keyboard( unsigned char k );

    /** shader changed event handler.
     * used when the user selects a new shader to draw with.
     * @param selected - index of the new selected shader.
     */
    void shaderChanged( GLuint selected );

    /** toggles edges.
     * Toggle models' edges drawing in edition mode.
     */
    void toggledrawmode() { m_modelmanager->toggledrawmode(); }

    /** toggles edition mode.
     * Edition mode draws a lot of edition stuff (gizmos, grid, edges, models for lights, ...).
     * When disables, nothing of these stuff is drew and the sky turns black.
     */
    void toggleEditionMode() { m_modelmanager->toggleEditionMode(); }

    /** Add a Model to the scene.
     *
     * @param type - type of the added Model.
     */
    void addModel( Model::ModelType type );

    /** Add a Light to the scene.
     *
     * @param type - type of the added Light.
     */
    void addLight( Light::LightType type );

    /** Delete the selected entity.
     *  Can either be a model or a light.
     */
    void deleteModel() { m_modelmanager->deleteModel(); }

    /** Selected model getter.
     * c.f. Model
     * @return nullptr if no model selected, the selected Model otherwise.
     */
    Model* getSelectedModel() { return m_modelmanager->getSelectedObject(); }

    /** Selected model's index getter.
     * c.f. Model
     * @return -1 if no model selected, the selected Model's index otherwise.
     */
    GLuint getSelectedIndex() { return m_modelmanager->getSelectedIndex(); }

    /** Selected model's index setter.
     *
     * @param index - new selected index.
     */
    void setSelectedIndex( GLuint index ) { m_modelmanager->setSelectedIndex( index ); }

    /** Selected light getter.
     * c.f. Light
     * @return nullptr if no light selected, the selected Light otherwise.
     */
    Light* getSelectedLight() { return m_modelmanager->getSelectedLight(); }

    /** Selected light's index getter.
     * c.f. Light
     * @return -1 if no light selected, the selected Light's index otherwise.
     */
    GLuint getSelectedLightIndex() { return m_modelmanager->getSelectedLightIndex(); }

    /** Selected model's settings setter.
     * c.f. Light
     * @param params - new settings of the selected model
     */
    void setModelParams( const Model::ModelParam& params );

    /** gizmo's type setter
     * c.f. Gizmo
     * @param type - new type of the gizmo.
     */
    void setGizmoType( Gizmo::GizmoType type ) { m_modelmanager->switchGizmo( type ); }

    void raytrace(const std::string &path) const;

  private:
    void checkCreation();

    int m_width;
    int m_height;

    struct ModelCreation {
        bool toCreate;
        Model::ModelType type;
    } m_creationstate;

    struct LightCreation {
        bool toCreate;
        Light::LightType type;
    } m_lightcreationstate;

    // geometries
    std::unique_ptr<ModelManager> m_modelmanager;

    // Shader
    using ShaderSelector = std::function<Shader*()>;
    std::vector<ShaderSelector> m_shaderselector;
    unsigned int m_activeshader;
    std::unique_ptr<Shader> m_shader;

    // mouse management
    int m_button{0}; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float m_mousex{0};
    float m_mousey{0};
    bool m_clicked_object{false};

    // Camera
    using CameraSelector = std::function<Camera*()>;
    std::vector<CameraSelector> m_cameraselector;
    unsigned int m_activecamera;
    std::unique_ptr<Camera> m_camera;

    glm::mat4 m_projection;
};

#endif // IIG3D_BASETP_HELLOSPHERES_HPP
