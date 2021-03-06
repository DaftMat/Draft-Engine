//
// Created by daftmat on 14/02/20.
//

#include "DirLight.hpp"

Light::LightParam DirLight::getParams() const {
    LightParam result{};
    result.dirlight.direction  = m_basedirection;
    result.spotlight.rotations = model().getRotation();
    return result;
}

void DirLight::editLight( const LightParam& params ) {
    m_basedirection = params.dirlight.direction;
    m_model->setRotation( params.dirlight.rotations );
    m_direction = m_model->rotation() * glm::vec4( m_basedirection, 0.f );
}

void DirLight::update() {
    LightParam params         = getParams();
    params.dirlight.rotations = m_model->getRotation();
    editLight( params );
}

void DirLight::setupModel() {
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices{1, 2, 2, 3, 3, 4, 4, 1, 1, 3, 2, 4};
    Mesh::Vertex vertex{};
    vertices.push_back( vertex );
    int index = 1;
    for ( float i = -0.2f; i <= 0.2f; i += 0.4f )
    {
        for ( float j = -0.2; j <= 0.2f; j += 0.4f )
        {
            glm::vec3 axisA{m_basedirection.y, m_basedirection.x, m_basedirection.z};
            glm::vec3 axisB = glm::cross( m_basedirection, axisA );
            vertex.Position = -m_basedirection * 2.f;
            vertex.Position += axisA * i + axisB * j;
            vertices.push_back( vertex );
            indices.push_back( 0 );
            indices.push_back( index++ );
        }
    }
    m_model->reset( vertices, indices );
}
