/**
 * Created by mathis on 04/03/2020.
 */

#include "ScaleGizmo.hpp"
void ScaleGizmo::init_models() {
    m_Xmodel.reset( init_arrow( glm::vec3( 1.f, 0.f, 0.f ) ) );
    m_Ymodel.reset( init_arrow( glm::vec3( 0.f, 1.f, 0.f ) ) );
    m_Zmodel.reset( init_arrow( glm::vec3( 0.f, 0.f, 1.f ) ) );
}

Model* ScaleGizmo::init_arrow( const glm::vec3& normal ) {
    glm::vec3 tangent{normal.z, normal.x, normal.y};
    glm::vec3 bitangent = glm::cross( normal, tangent );
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    float scale = 0.01f;
    float dist  = 4.f;

    Vertex vertex{};
    vertex.Position = tangent * scale;
    vertices.push_back( vertex );
    vertex.Position += normal;
    vertices.push_back( vertex );
    vertex.Position = glm::normalize( tangent + bitangent ) * scale;
    vertices.push_back( vertex );
    vertex.Position += normal;
    vertices.push_back( vertex );
    vertex.Position = bitangent * scale;
    vertices.push_back( vertex );
    vertex.Position += normal;
    vertices.push_back( vertex );
    vertex.Position = glm::normalize( bitangent - tangent ) * scale;
    vertices.push_back( vertex );
    vertex.Position += normal;
    vertices.push_back( vertex );
    vertex.Position = -tangent * scale;
    vertices.push_back( vertex );
    vertex.Position += normal;
    vertices.push_back( vertex );
    vertex.Position = glm::normalize( -tangent - bitangent ) * scale;
    vertices.push_back( vertex );
    vertex.Position += normal;
    vertices.push_back( vertex );
    vertex.Position = -bitangent * scale;
    vertices.push_back( vertex );
    vertex.Position += normal;
    vertices.push_back( vertex );
    vertex.Position = glm::normalize( tangent - bitangent ) * scale;
    vertices.push_back( vertex );
    vertex.Position += normal;
    vertices.push_back( vertex );

    vertex.Position = tangent * dist * scale + normal;
    vertices.push_back( vertex );
    vertex.Position += normal * 8.f * scale;
    vertices.push_back( vertex );
    vertex.Position = glm::normalize( tangent + bitangent ) * dist * scale + normal;
    vertices.push_back( vertex );
    vertex.Position += normal * 8.f * scale;
    vertices.push_back( vertex );
    vertex.Position = bitangent * dist * scale + normal;
    vertices.push_back( vertex );
    vertex.Position += normal * 8.f * scale;
    vertices.push_back( vertex );
    vertex.Position = glm::normalize( bitangent - tangent ) * dist * scale + normal;
    vertices.push_back( vertex );
    vertex.Position += normal * 8.f * scale;
    vertices.push_back( vertex );
    vertex.Position = -tangent * dist * scale + normal;
    vertices.push_back( vertex );
    vertex.Position += normal * 8.f * scale;
    vertices.push_back( vertex );
    vertex.Position = glm::normalize( -tangent - bitangent ) * dist * scale + normal;
    vertices.push_back( vertex );
    vertex.Position += normal * 8.f * scale;
    vertices.push_back( vertex );
    vertex.Position = -bitangent * dist * scale + normal;
    vertices.push_back( vertex );
    vertex.Position += normal * 8.f * scale;
    vertices.push_back( vertex );
    vertex.Position = glm::normalize( tangent - bitangent ) * dist * scale + normal;
    vertices.push_back( vertex );
    vertex.Position += normal * 8.f * scale;
    vertices.push_back( vertex );

    for ( int i = 0; i < 16; i += 2 )
    {
        indices.push_back( i );
        indices.push_back( ( i + 3 ) % 16 );
        indices.push_back( i + 1 );
        indices.push_back( i );
        indices.push_back( ( i + 2 ) % 16 );
        indices.push_back( ( i + 3 ) % 16 );
    }
    for ( int i = 16; i < 32; i += 2 )
    {
        indices.push_back( i );
        indices.push_back( ( i + 3 ) % 16 + 16 );
        indices.push_back( i + 1 );
        indices.push_back( i );
        indices.push_back( ( i + 2 ) % 16 + 16 );
        indices.push_back( ( i + 3 ) % 16 + 16 );
    }
    return new Model( vertices, indices );
}

void ScaleGizmo::scale( float scale ) {
    m_Xmodel->setScale( glm::vec3( scale ) );
    m_Ymodel->setScale( glm::vec3( scale ) );
    m_Zmodel->setScale( glm::vec3( scale ) );
}

void ScaleGizmo::move( float xpos,
                       float ypos,
                       Model& model,
                       const glm::mat4& projection,
                       const glm::mat4& view ) {
    glm::vec3 dir       = getDir();
    float xoffset       = xpos - m_xmouse;
    float yoffset       = ypos - m_ymouse;
    m_xmouse            = xpos;
    m_ymouse            = ypos;
    glm::vec2 mousevec  = {xoffset, yoffset};
    glm::vec2 dirscreen = glm::normalize( glm::vec2( projection * view * glm::vec4( dir, 0.0 ) ) );
    float angle         = glm::dot( mousevec, dirscreen );
    model.updateScale( angle * dir / 100.f );
    setTransform( model );
}

glm::vec3 ScaleGizmo::getDir() {
    switch ( m_selected )
    {
    case XSELEC:
        return glm::vec3( 1.f, 0.f, 0.f );
    case YSELEC:
        return glm::vec3( 0.f, 1.f, 0.f );
    case ZSELEC:
        return glm::vec3( 0.f, 0.f, 1.f );
    default:
        return glm::vec3( 0.f, 0.f, 0.f );
    }
}