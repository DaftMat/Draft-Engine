//
// Created by mathis on 01/03/2020.
//

#ifndef DAFT_ENGINE_GIZMO_HPP
#define DAFT_ENGINE_GIZMO_HPP

#include <Engine/Shader.hpp>
#include <opengl_stuff.h>
#include <src/Engine/Geometry/Model.hpp>
#include <src/Utils/types.hpp>

enum Selection { XSELEC, YSELEC, ZSELEC, NONE };

enum GizmoType { TRANSLATE, SCALE };

/**
 * A nice post-GIMP Scene handler.
 */
class Gizmo
{
  public:
    /** Constructor.
     *
     * @param transform - transformation applied to the affiliated object.
     */
    Gizmo( const Utils::Transform& transform ) : m_transform{transform}, m_selected{NONE} {}
    virtual ~Gizmo();

    /** Draws the Model of the gizmo.
     *
     * @param projection - projection matrix of the Camera.
     * @param view - view matrix of the Camera.
     */
    void draw( const glm::mat4& projection, const glm::mat4& view );

    /** X axis Model 's OBB getter. */
    Obb getXobb() { return m_Xmodel->obb(); }
    /** Y axis Model 's OBB getter. */
    Obb getYobb() { return m_Ymodel->obb(); }
    /** Z axis Model 's OBB getter. */
    Obb getZobb() { return m_Zmodel->obb(); }
    /** Change the Selection of the Axis. */
    void setSelected( Selection selec ) { m_selected = selec; }
    /**
     *
     * @return false if no Axis selected.
     */
    bool isSelected() { return m_selected != NONE; }

    /** Click on gizmo event.
     *  Sets the current mouse position for later offset calculation.
     * @param xpos - x position of the click.
     * @param ypos - y position of the click.
     */
    void clicked( float xpos, float ypos ) { m_xmouse = xpos, m_ymouse = ypos; }

    /** Move action when a Model is selected.
     *
     * @param xpos - new mouse's x position
     * @param ypos - new mouse's y position
     * @param model - affiliated Model
     * @param projection - projection matrix of the Camera.
     * @param view - view matrix of the Camera.
     */
    virtual void move( float xpos,
                       float ypos,
                       Model& model,
                       const glm::mat4& projection,
                       const glm::mat4& view ) = 0;

    /** Move action when a Light is selected.
     *
     * @param xpos - new mouse's x position
     * @param ypos - new mouse's y position
     * @param light - affiliated Light
     * @param projection - projection matrix of the Camera.
     * @param view - view matrix of the Camera.
     */
    virtual void move( float xpos,
                       float ypos,
                       Light& light,
                       const glm::mat4& projection,
                       const glm::mat4& view ) = 0;

    /** Scale the gizmo.
     *  Used before drawing.
     * @param scale - new scale
     */
    virtual void scale( float scale ) = 0;
    /** gizmo's type getter */
    virtual GizmoType type() const = 0;

    void setTransform( const Model& model );
    const Utils::Transform& getTransform() const { return m_transform; }

  protected:
    std::unique_ptr<Model> m_Xmodel;
    std::unique_ptr<Model> m_Ymodel;
    std::unique_ptr<Model> m_Zmodel;
    Selection m_selected;

    float m_xmouse;
    float m_ymouse;

  private:
    virtual void init_models() = 0;
    // rotation/position
    Utils::Transform m_transform;

    std::unique_ptr<Shader> m_shader{
        new Shader( "shaders/color.vert.glsl", "shaders/color.frag.glsl" )};
};

#endif // DAFT_ENGINE_GIZMO_HPP
