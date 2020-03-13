/**
 * Created by mathis on 13/02/2020.
 */

#include "PointLight.hpp"

Light::LightParam PointLight::getParams() const {
    LightParam result{};
    result.pointlight.position  = m_position;
    result.pointlight.intensity = m_intensity;
    return result;
}

void PointLight::editLight( const LightParam& params ) {
    m_position  = params.pointlight.position;
    m_intensity = params.pointlight.intensity;
    m_model->setPosition( m_position );
}

void PointLight::setupModel() {
    std::vector<glm::vec3> dir{
        {0.f, 0.f, 1.f},  // front
        {0.f, 0.f, -1.f}, // back
        {0.f, 1.f, 0.f},  // top
        {0.f, -1.f, 0.f}, // bottom
        {1.f, 0.f, 0.f},  // right
        {-1.f, 0.f, 0.f}  // left
    };

    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices;
    Mesh::Vertex vertex{};
    vertices.push_back( vertex );

    for ( int i = 0; i < 6; ++i )
    {
        vertex.Position = dir[i] * 0.5f;
        vertices.push_back( vertex );
        indices.push_back( 0 );
        indices.push_back( i + 1 );
    }

    m_model->reset( vertices, indices );
}
