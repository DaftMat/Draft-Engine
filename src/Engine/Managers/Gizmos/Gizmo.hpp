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

enum GizmoType { TRANSLATE, ROTATE, SCALE };

class Gizmo
{
  public:
    Gizmo( const Utils::Transform& transform ) : m_transform{transform}, m_selected{NONE} {}
    virtual ~Gizmo() {
        m_Xmodel.reset( nullptr ), m_Ymodel.reset( nullptr ), m_Zmodel.reset( nullptr ),
            m_shader.reset( nullptr );
    }

    void draw( const glm::mat4& projection, const glm::mat4& view );

    Obb getXobb() { return m_Xmodel->obb(); }
    Obb getYobb() { return m_Ymodel->obb(); }
    Obb getZobb() { return m_Zmodel->obb(); }
    void setSelected( Selection selec ) { m_selected = selec; }
    bool isSelected() { return m_selected != NONE; }

    void clicked( float xpos, float ypos ) { m_xmouse = xpos, m_ymouse = ypos; }
    virtual void move( float xpos,
                       float ypos,
                       Model& model,
                       const glm::mat4& projection,
                       const glm::mat4& view ) = 0;
    virtual void move( float xpos,
                       float ypos,
                       Light& light,
                       const glm::mat4& projection,
                       const glm::mat4& view ) = 0;

    virtual void scale( float scale ) = 0;
    virtual GizmoType type() const    = 0;

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
