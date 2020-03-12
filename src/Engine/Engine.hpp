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

/** Engine Class.
 *  Defines the whole drew scene.
 */
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

    /** draw the scene. */
    void draw();

    void mouseclick( int button, float xpos, float ypos );
    void mouserelease();
    void mousemove( float xpos, float ypos );
    void keyboardmove( int key, double time );
    bool keyboard( unsigned char k );
    void shaderChanged( GLuint selected );

    void toggledrawmode() { m_modelmanager->toggledrawmode(); }
    void toggleEditionMode() { m_modelmanager->toggleEditionMode(); }

    /** Add a Model to the scene.
     *
     * @param type - type of the added Model.
     */
    void addModel( ModelType type );

    /** Add a Light to the scene.
     *
     * @param type - type of the added Light.
     */
    void addLight( LightType type );

    /** Delete the selected entity.
     *  Can either be a model or a light.
     */
    void deleteModel() { m_modelmanager->deleteModel(); }

    /** Selected Model getter.
     *
     * @return nullptr if no model selected, the selected Model otherwise.
     */
    Model* getSelectedModel() { return m_modelmanager->getSelectedObject(); }

    /** Selected Model's index getter.
     *
     * @return -1 if no model selected, the selected Model's index otherwise.
     */
    GLuint getSelectedIndex() { return m_modelmanager->getSelectedIndex(); }

    /** Selected Model's index setter */
    void setSelectedIndex( GLuint index ) { m_modelmanager->setSelectedIndex( index ); }

    /** Selected Light getter.
     *
     * @return nullptr if no light selected, the selected Light otherwise.
     */
    Light* getSelectedLight() { return m_modelmanager->getSelectedLight(); }

    /** Selected Light's index getter.
     *
     * @return -1 if no light selected, the selected Light's index otherwise.
     */
    GLuint getSelectedLightIndex() { return m_modelmanager->getSelectedLightIndex(); }

    /** Selected Model's settings setter. */
    void setModelParams( const ModelParam& params );

    /** Gizmo's type setter */
    void setGizmoType( GizmoType type ) { m_modelmanager->switchGizmo( type ); }

  private:
    void checkCreation();

    int m_width;
    int m_height;

    struct ModelCreation {
        bool toCreate;
        ModelType type;
    } m_creationstate;

    struct LightCreation {
        bool toCreate;
        LightType type;
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
