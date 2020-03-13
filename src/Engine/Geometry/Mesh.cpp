/**
 * Created by mathis on 01/02/2020.
 */

#include "Mesh.hpp"
#include "src/Engine/Shader.hpp"
#include <thread>
#include <unistd.h>

Mesh::Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices, bool grid ) :
    m_vertices{std::move( vertices )},
    m_indices{std::move( indices )},
    m_grid{grid} {
    setupMesh();
}

Mesh::~Mesh() {
    deleteMesh();
    m_vertices.clear();
    m_indices.clear();
}

void Mesh::setupMesh() {
    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_VBO );
    glGenBuffers( 1, &m_EBO );
    /// open
    glBindVertexArray( m_VAO );

    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData(
        GL_ARRAY_BUFFER, m_vertices.size() * sizeof( Vertex ), m_vertices.data(), GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                  m_indices.size() * sizeof( GLuint ),
                  m_indices.data(),
                  GL_STATIC_DRAW );

    /// in vertex position
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)nullptr );
    glEnableVertexAttribArray( 0 );
    /// in vertex normal
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Normal ) );
    glEnableVertexAttribArray( 1 );
    /// in vertex texture coords
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, TexCoords ) );
    glEnableVertexAttribArray( 2 );

    /// release
    glBindVertexArray( 0 );
}

void Mesh::deleteMesh() {
    glDeleteVertexArrays( 1, &m_VAO );
    glDeleteBuffers( 1, &m_VBO );
    glDeleteBuffers( 1, &m_EBO );
}

void Mesh::reset( std::vector<Vertex> vertices, std::vector<GLuint> indices ) {
    m_vertices = std::move( vertices );
    m_indices  = std::move( indices );
    deleteMesh();
    setupMesh();
}

void Mesh::draw() const {
    glBindVertexArray( m_VAO );
    glDrawElements( m_grid ? GL_LINES : GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr );
    glBindVertexArray( 0 );
}

Utils::Aabb Mesh::aabb() const {
    Utils::Aabb ret;
    for ( const auto& v : m_vertices )
    {
        ret.extend( toEigen( v.Position ) );
    }
    return ret;
}
