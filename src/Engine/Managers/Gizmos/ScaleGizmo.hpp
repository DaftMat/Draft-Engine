/**
 * Created by mathis on 04/03/2020.
 */

#ifndef DAFT_ENGINE_SCALEGIZMO_HPP
#define DAFT_ENGINE_SCALEGIZMO_HPP

#include "Gizmo.hpp"

class ScaleGizmo : public Gizmo {
public:
    ScaleGizmo(const Utils::Transform &transform) : Gizmo(transform) { init_models(); }

    void move(float xoffset, float yoffset, Model &model, const glm::mat4 &projection, const glm::mat4 &view) override;
    void scale(float scale) override;
    GizmoType type() const override { return SCALE; }

private:
    void init_models() override;
    Model * init_arrow(const glm::vec3 &normal);
    glm::vec3 getDir();
};


#endif //DAFT_ENGINE_SCALEGIZMO_HPP
