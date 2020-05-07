/**
 * Created by mathis on 01/03/2020.
 */

#include "Gizmo.hpp"

#include <Utils/adapters.hpp>

Gizmo::~Gizmo() {
    m_Xmodel.reset( nullptr );
    m_Ymodel.reset( nullptr );
    m_Zmodel.reset( nullptr );
    m_shader.reset( nullptr );
}

void Gizmo::draw( float dt, const glm::mat4& projection, const glm::mat4& view ) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    m_shader->use();
    m_shader->setMat4( "projection", projection );
    m_shader->setMat4( "view", view );
    m_shader->setVec3( "color", glm::vec3( 1.f, 0.f, 0.f ) );
    if ( m_selected == XSELEC ) m_shader->setVec3( "color", glm::vec3( 0.f, 1.f, 1.f ) );
    m_Xmodel->draw( dt, *m_shader );
    m_shader->setVec3( "color", glm::vec3( 0.f, 1.f, 0.f ) );
    if ( m_selected == YSELEC ) m_shader->setVec3( "color", glm::vec3( 0.f, 1.f, 1.f ) );
    m_Ymodel->draw( dt, *m_shader );
    m_shader->setVec3( "color", glm::vec3( 0.f, 0.f, 1.f ) );
    if ( m_selected == ZSELEC ) m_shader->setVec3( "color", glm::vec3( 0.f, 1.f, 1.f ) );
    m_Zmodel->draw( dt, *m_shader );
}

void Gizmo::setTransform( const Model& model ) {
    m_Xmodel->setPosition( model.getPosition() );
    m_Ymodel->setPosition( model.getPosition() );
    m_Zmodel->setPosition( model.getPosition() );

    m_Xmodel->setRotation( model.getRotation() );
    m_Ymodel->setRotation( model.getRotation() );
    m_Zmodel->setRotation( model.getRotation() );
}
