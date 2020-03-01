/**
 * Created by mathis on 01/03/2020.
 */

#ifndef DAFT_ENGINE_GIZMO_HPP
#define DAFT_ENGINE_GIZMO_HPP

#include <opengl_stuff.h>
#include <src/Utils/types.hpp>
#include <src/Engine/Geometry/Model.hpp>

enum Selection {
    XSELEC,
    YSELEC,
    ZSELEC,
    NONE
};

class Gizmo {
public:
    Gizmo(const Utils::Transform &transform) : m_transform { transform }, m_selected { NONE } {}
    virtual ~Gizmo() { m_Xmodel.reset(nullptr), m_Ymodel.reset(nullptr), m_Zmodel.reset(nullptr), m_shader.reset(nullptr); }

    const Utils::Transform & transform() const { return m_transform; }
    Utils::Transform & transform() { return m_transform; }

    void draw(const glm::mat4 &projection, const glm::mat4 &view);

    Obb getXobb() { return m_Xmodel->obb(); }
    Obb getYobb() { return m_Ymodel->obb(); }
    Obb getZobb() { return m_Zmodel->obb(); }
    void setSelected(Selection selec) { m_selected = selec; }

    virtual void move(float xoffset, float yoffset) = 0;
    virtual void scale(float scale) = 0;

protected:
    std::unique_ptr<Model> m_Xmodel;
    std::unique_ptr<Model> m_Ymodel;
    std::unique_ptr<Model> m_Zmodel;

private:
    virtual void init_models() = 0;
    // rotation/position
    Utils::Transform m_transform;
    Selection m_selected;

    std::unique_ptr<Shader> m_shader { new Shader("shaders/color.vert.glsl", "shaders/color.frag.glsl") };
};


#endif //DAFT_ENGINE_GIZMO_HPP
