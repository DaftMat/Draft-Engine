//
// Created by mathis on 01/03/2020.
//

#ifndef DAFT_ENGINE_GIZMO_HPP
#define DAFT_ENGINE_GIZMO_HPP

#include <Engine/Shader.hpp>
#include <opengl_stuff.h>
#include <src/Engine/Geometry/Model.hpp>
#include <src/Utils/types.hpp>

/** A nice post-GIMP Scene handler. */
class Gizmo
{
  public:
    /** Defines what axis is currently being moved by the mouse */
    enum Selection { XSELEC, YSELEC, ZSELEC, NONE };

    /** Defines how the gizmo transform the concerned model or light */
    enum GizmoType { TRANSLATE, SCALE };

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

    /** X axis Model 's OBB getter.
     * c.f. Obb - used for mouse click detection.
     * @return the model's OBB of the gizmo's X axis.
     */
    Obb getXobb() { return m_Xmodel->obb(); }

    /** Y axis Model 's OBB getter.
     * c.f. Obb - used for mouse click detection.
     * @return the model's OBB of the gizmo's Y axis.
     */
    Obb getYobb() { return m_Ymodel->obb(); }

    /** Z axis Model 's OBB getter.
     * c.f. Obb - used for mouse click detection.
     * @return the model's OBB of the gizmo's Z axis.
     */
    Obb getZobb() { return m_Zmodel->obb(); }

    /** Change the Selection of the Axis.
     * c.f. Selection
     * @param selec - new selection.
     */
    void setSelected( Selection selec ) { m_selected = selec; }

    /** Is the gizmo being moved.
     * used to check if any axis is clicked.
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
     * @param xpos - new mouse's x position.
     * @param ypos - new mouse's y position.
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
     * @param xpos - new mouse's x position.
     * @param ypos - new mouse's y position.
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
     * @param scale - new scale.
     */
    virtual void scale( float scale ) = 0;

    /** gizmo's type getter.
     * c.f. GizmoType
     * @return type of the gizmo.
     */
    virtual GizmoType type() const = 0;

    /** gizmo's transform setter.
     * set the gizmo's transformation to a Model 's transformation.
     * @param model - model with the right transformation.
     */
    void setTransform( const Model& model );

    /** gizmo's transform getter.
     *
     * @return gizmo's transformation.
     */
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
