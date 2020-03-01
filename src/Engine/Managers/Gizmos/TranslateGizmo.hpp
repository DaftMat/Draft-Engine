/**
 * Created by mathis on 01/03/2020.
 */

#ifndef DAFT_ENGINE_TRANSLATEGIZMO_HPP
#define DAFT_ENGINE_TRANSLATEGIZMO_HPP

#include "Gizmo.hpp"

class TranslateGizmo : public Gizmo {
public:
    TranslateGizmo(const Utils::Transform &transform) : Gizmo(transform) { init_models(); }

    void move(float xoffset, float yoffset) override {}
    void scale(float scale) override;

private:
    void init_models() override;
    Model * init_arrow(const glm::vec3 &normal);
};


#endif //DAFT_ENGINE_TRANSLATEGIZMO_HPP
