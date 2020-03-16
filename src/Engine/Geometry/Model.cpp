//
// Created by daftmat on 10/02/20.
//

#include "Model.hpp"
#include <Engine/Shader.hpp>

Model::Model( const std::vector<Mesh::Vertex>& vertices,
              const std::vector<GLuint>& indices,
              bool wire ) :
    m_wire{wire} {
    m_meshes.clear();
    m_meshes.emplace_back( new Mesh( vertices, indices, m_wire ) );
}

void Model::draw( const Shader& shader ) const {
    shader.use();
    shader.setMat4( "model", model() );
    shader.setMaterial( m_material );

    for ( auto& mesh : m_meshes )
    {
        mesh->draw();
    }
}

void Model::setPosition( const glm::vec3& new_pos ) {
    m_position     = new_pos;
    m_translateMat = glm::translate( glm::mat4(), m_position );
}

void Model::setRotation( const glm::vec3& new_rot ) {
    m_rotation  = new_rot;
    m_rotateMat = rotation();
}

void Model::setScale( const glm::vec3& new_scale ) {
    m_scale = new_scale;
}

glm::mat4 Model::rotation() const {
    glm::vec3 x{1.f, 0.f, 0.f};
    glm::vec3 y{0.f, 1.f, 0.f};
    glm::vec3 z{0.f, 0.f, 1.f};
    glm::mat4 res;
    res = glm::rotate( res, glm::radians( m_rotation.x ), x );
    res = glm::rotate( res, glm::radians( m_rotation.y ), y );
    res = glm::rotate( res, glm::radians( m_rotation.z ), z );
    return res;
}

void Model::translate( const glm::vec3& t ) {
    setPosition( m_position + t );
}

Utils::Aabb Model::base_aabb() const {
    Utils::Aabb aabb;
    for ( const auto& m : m_meshes )
    {
        aabb.extend( m->aabb() );
    }
    if ( aabb.isEmpty() ) return aabb;
    Utils::Aabb res;
    for ( int i = 0; i < 8; ++i )
    {
        res.extend( Utils::Transform( toEigen( scale() ) ) *
                    aabb.corner( Utils::Aabb::CornerType( i ) ) );
    }
    return res;
}

Utils::Aabb Model::aabb() const {
    Utils::Aabb aabb;
    for ( const auto& m : m_meshes )
    {
        aabb.extend( m->aabb() );
    }
    if ( aabb.isEmpty() ) return aabb;
    Utils::Aabb res;
    for ( int i = 0; i < 8; ++i )
    {
        res.extend( Utils::Transform( toEigen( model() ) ) *
                    aabb.corner( Utils::Aabb::CornerType( i ) ) );
    }
    return res;
}

GLuint Model::getSize() const {
    GLuint size = 0;
    for ( const auto& mesh : m_meshes )
    {
        size += mesh->getSize();
    }
    return size;
}