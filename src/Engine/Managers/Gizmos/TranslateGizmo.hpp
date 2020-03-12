//
// Created by mathis on 01/03/2020.
//

#ifndef DAFT_ENGINE_TRANSLATEGIZMO_HPP
#define DAFT_ENGINE_TRANSLATEGIZMO_HPP

#include "Gizmo.hpp"

class TranslateGizmo : public Gizmo
{
  public:
    TranslateGizmo( const Utils::Transform& transform ) : Gizmo( transform ) { init_models(); }

    void move( float xpos,
               float ypos,
               Model& model,
               const glm::mat4& projection,
               const glm::mat4& view ) override;
    void move( float xpos,
               float ypos,
               Light& light,
               const glm::mat4& projection,
               const glm::mat4& view ) override;
    void scale( float scale ) override;
    GizmoType type() const override { return TRANSLATE; }

  private:
    void init_models() override;
    Model* init_arrow( const glm::vec3& normal );
    glm::vec3 getDir( const glm::mat4& rotation );
};

#endif // DAFT_ENGINE_TRANSLATEGIZMO_HPP
