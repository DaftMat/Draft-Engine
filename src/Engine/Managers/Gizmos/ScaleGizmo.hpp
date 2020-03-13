//
// Created by mathis on 04/03/2020.
//

#ifndef DAFT_ENGINE_SCALEGIZMO_HPP
#define DAFT_ENGINE_SCALEGIZMO_HPP

#include "Gizmo.hpp"

/** gizmo to scale objects. */
class ScaleGizmo : public Gizmo
{
  public:
    /** Constructor.
     *
     * @param transform - transformation applied to the affiliated object.
     */
    ScaleGizmo( const Utils::Transform& transform ) : Gizmo( transform ) { init_models(); }

    /** Move action when a Model is selected.
     *
     * @param xpos - new mouse's x position.
     * @param ypos - new mouse's y position.
     * @param model - affiliated Model
     * @param projection - projection matrix of the Camera.
     * @param view - view matrix of the Camera.
     */
    void move( float xpos,
               float ypos,
               Model& model,
               const glm::mat4& projection,
               const glm::mat4& view ) override;

    /** Empty override.
     * No scale action for lights.
     */
    void move( float, float, Light&, const glm::mat4&, const glm::mat4& ) override {}

    /** Scale the gizmo.
     *  Used before drawing.
     * @param scale - new scale.
     */
    void scale( float scale ) override;

    /** gizmo's type getter.
     * c.f. GizmoType
     * @return type of the gizmo.
     */
    GizmoType type() const override { return SCALE; }

  private:
    void init_models() override;
    Model* init_arrow( const glm::vec3& normal );
    glm::vec3 getDir();
};

#endif // DAFT_ENGINE_SCALEGIZMO_HPP
