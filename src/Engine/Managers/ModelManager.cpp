/**
 * Created by mathis on 12/02/2020.
 */

#include "ModelManager.hpp"
#include <Engine/Geometry/Lights/DirLight.hpp>
#include <Engine/Geometry/Lights/SpotLight.hpp>
#include <Engine/Geometry/Primitives/Cube.hpp>
#include <Engine/Geometry/Primitives/CubeSphere.hpp>
#include <Engine/Geometry/Primitives/IcoSphere.hpp>
#include <Engine/Geometry/Primitives/UVSphere.hpp>

void ModelManager::draw( Shader& shader,
                         const glm::mat4& view,
                         const glm::mat4& projection,
                         const glm::vec3& viewPos,
                         const glm::vec3& viewDir ) {
    if ( m_edition )
        glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
    else
        glClearColor( 0.f, 0.f, 0.f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT );

    if ( m_edition ) drawGrid( projection, view );

    if ( m_models.empty() && m_lights.empty() ) return;

    if ( ( m_selectedmodel > -1 || m_selectedlight > -1 ) && m_edition )
    {
        updateGizmo( viewPos );
        glDepthRange( 0.0, 0.01 );
        m_gizmo->draw( projection, view );
        glDepthRange( 0.01, 1.0 );
    }

    for ( const auto& ind : m_toReset )
        m_models[ind]->reset();
    m_toReset.clear();

    shader.use();
    shader.setVec3( "viewPos", viewPos );
    shader.setMat4( "view", view );
    shader.setMat4( "projection", projection );
    for ( const auto& light : m_lights )
    {
        shader.addLight( light.get() );
    }

    if ( m_edition )
    {
        Light::LightParam new_dir  = m_editionlight->getParams();
        new_dir.dirlight.direction = viewDir;
        m_editionlight->editLight( new_dir );
        shader.addLight( m_editionlight.get() );
    }

    for ( GLuint i = 0; i < m_models.size(); ++i )
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        m_models[i]->draw( shader );
        if ( m_edition )
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            m_colorshader->use();
            if ( i == m_selectedmodel )
            {
                m_colorshader->setVec3( "color", glm::vec3( 0.f, 1.f, 1.f ) );
                m_models[i]->draw( *m_colorshader );
            }
            else if ( m_wireframe )
            {
                m_colorshader->setVec3( "color", glm::vec3( 0.f, 0.f, 0.f ) );
                m_models[i]->draw( *m_colorshader );
            }
        }
    }

    if ( m_edition )
    {
        m_colorshader->use();
        for ( GLuint i = 0; i < m_lights.size(); ++i )
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            if ( i == m_selectedlight )
            {
                m_colorshader->setVec3( "color", glm::vec3( 0.f, 1.f, 1.f ) );
                m_lights[i]->reset();
                m_lights[i]->model().draw( *m_colorshader );
            }
            else if ( m_wireframe )
            {
                m_colorshader->setVec3( "color", glm::vec3( 0.9f, 0.9f, 0.9f ) );
                m_lights[i]->reset();
                m_lights[i]->model().draw( *m_colorshader );
            }
        }
    }

    glDepthRange( 0.0, 1.0 );

    shader.clearLights();
}

void ModelManager::addLight( Light::LightType type ) {
    switch ( type )
    {
    case Light::POINT_LIGHT:
        m_lights.emplace_back( new PointLight() );
        break;
    case Light::DIR_LIGHT:
        m_lights.emplace_back( new DirLight() );
        break;
    case Light::SPOT_LIGHT:
        m_lights.emplace_back( new SpotLight() );
        break;
    default:
        break;
    }
}

void ModelManager::addObject( Model::ModelType type ) {
    switch ( type )
    {
    case Model::UV_SPHERE:
        m_models.emplace_back( new UVSphere() );
        break;
    case Model::ICO_SPHERE:
        m_models.emplace_back( new IcoSphere() );
        break;
    case Model::CUBE_SPHERE:
        m_models.emplace_back( new CubeSphere() );
        break;
    case Model::CUBE:
        m_models.emplace_back( new Cube() );
        break;
    default:
        break;
    }
}

void ModelManager::setObjectParams( const Model::ModelParam& params ) {
    m_models[m_selectedmodel]->editModel( params );
    m_toReset.insert( m_selectedmodel );
}

bool ModelManager::keyboard( unsigned char key ) {
    switch ( key )
    {
    case 'd':
        deleteModel();
        return true;
    default:
        return false;
    }
}

void ModelManager::makeGrid( int size ) {
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices;

    GLuint index;
    Mesh::Vertex vertex{};
    glm::vec3 normal{0.f, 1.f, 0.f};
    for ( int i = 0; i < size; ++i )
    {
        for ( int j = 0; j < size; ++j )
        {
            index         = ( GLuint )( i + j * size );
            vertex.Normal = normal;
            vertex.Position =
                glm::vec3( (float)( i - ( size / 2 ) ), 0.f, (float)( j - ( size / 2 ) ) );
            vertices.push_back( vertex );

            if ( i < size && j < size && ( i != size / 2 || j != size / 2 ) )
            {
                if ( i < size - 1 )
                {
                    indices.push_back( index );
                    indices.push_back( index + 1 );
                }
                if ( j < size - 1 )
                {
                    indices.push_back( index );
                    indices.push_back( index + size );
                }
            }
        }
    }

    m_grid.reset( new Mesh( vertices, indices, true ) );
}

void ModelManager::drawGrid( const glm::mat4& projection, const glm::mat4& view ) {
    m_colorshader->use();
    m_colorshader->setMat4( "projection", projection );
    m_colorshader->setMat4( "view", view );
    m_colorshader->setMat4( "model", glm::mat4() );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    /// Draw grid
    m_colorshader->setVec3( "color", glm::vec3( 0.25f, 0.25f, 0.25f ) );
    m_grid->draw();
    /// Draw unit arrows
    for ( int i = 0; i < 3; ++i )
    {
        m_colorshader->setMat4( "model", glm::mat4() );
        m_colorshader->setVec3( "color", dirs()[i] );
        m_unitarrows[i]->draw();
    }
}

void ModelManager::makeUnitArrows() {
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices{0, 1};
    Mesh::Vertex vertex{};
    Mesh::Vertex center{};
    for ( int i = 0; i < 3; ++i )
    {
        vertices.push_back( center );
        vertex.Position = dirs()[i];
        vertices.push_back( vertex );
        m_unitarrows.emplace_back( new Mesh( vertices, indices, true ) );
        vertices.clear();
    }
}

void ModelManager::deleteModel() {
    if ( m_selectedmodel > -1 )
    {
        for ( auto it = m_models.begin(); it != m_models.end(); ++it )
        {
            if ( *it == m_models[m_selectedmodel] )
            {
                m_models.erase( it );
                m_selectedmodel = m_models.size() - 1;
                return;
            }
        }
    }
    else if ( m_selectedlight > -1 )
    {
        for ( auto it = m_lights.begin(); it != m_lights.end(); ++it )
        {
            if ( *it == m_lights[m_selectedlight] )
            {
                m_lights.erase( it );
                m_selectedlight = m_lights.size() - 1;
                return;
            }
        }
    }
}

bool ModelManager::mouse_click( const Ray& ray, float xpos, float ypos ) {
    float dist = 100000.f;
    if ( m_selectedmodel != -1 || m_selectedlight != -1 )
    {
        bool found = false;
        if ( ray.intersects( m_gizmo->getXobb(), dist ) )
        {
            m_gizmo->setSelected( Gizmo::XSELEC );
            found = true;
        }
        else if ( ray.intersects( m_gizmo->getYobb(), dist ) )
        {
            m_gizmo->setSelected( Gizmo::YSELEC );
            found = true;
        }
        else if ( ray.intersects( m_gizmo->getZobb(), dist ) )
        {
            m_gizmo->setSelected( Gizmo::ZSELEC );
            found = true;
        }
        else
        { m_gizmo->setSelected( Gizmo::NONE ); }
        if ( found )
        {
            m_gizmo->clicked( xpos, ypos );
            return found;
        }
    }
    bool found;
    found = false;
    for ( int i = 0; i < m_models.size(); ++i )
    {
        float temp;
        if ( ray.intersects( m_models[i]->obb(), temp ) )
        {
            found = true;
            if ( temp < dist )
            {
                dist            = temp;
                m_selectedmodel = i;
            }
        }
    }
    if ( !found )
        m_selectedmodel = -1;
    else
        m_selectedlight = -1;
    bool found_light = false;
    for ( int i = 0; i < m_lights.size(); ++i )
    {
        float temp;
        if ( ray.intersects( m_lights[i]->model().obb(), temp ) )
        {
            if ( temp < dist )
            {
                found_light     = true;
                dist            = temp;
                m_selectedlight = i;
            }
        }
    }
    if ( !found_light )
        m_selectedlight = -1;
    else
        m_selectedmodel = -1;
    return found || found_light;
}

void ModelManager::updateGizmo( const glm::vec3& viewPos ) {
    if ( m_gizmoType != m_gizmo->type() ) switchGizmo();
    if ( m_selectedmodel > -1 && m_selectedmodel < m_models.size() )
    {
        m_gizmo->setTransform( *m_models[m_selectedmodel] );
        m_gizmo->scale( glm::length( viewPos - m_models[m_selectedmodel]->getPosition() ) / 3.f );
    }
    else if ( m_selectedlight > -1 && m_selectedlight < m_lights.size() )
    {
        m_gizmo->setTransform( m_lights[m_selectedlight]->model() );
        m_gizmo->scale( glm::length( viewPos - m_lights[m_selectedlight]->model().getPosition() ) /
                        3.f );
    }
}

void ModelManager::mouse_move( float xpos,
                               float ypos,
                               const glm::mat4& projection,
                               const glm::mat4& view ) {
    if ( m_gizmo->isSelected() )
    {
        if ( m_selectedmodel > -1 )
            m_gizmo->move( xpos, ypos, *m_models[m_selectedmodel], projection, view );
        else if ( m_selectedlight > -1 )
            m_gizmo->move( xpos, ypos, *m_lights[m_selectedlight], projection, view );
    }
}

void ModelManager::mouserelease() {
    m_gizmo->setSelected( Gizmo::NONE );
}

void ModelManager::switchGizmo() {
    switch ( m_gizmoType )
    {
    case Gizmo::TRANSLATE:
        m_gizmo.reset( new TranslateGizmo( Utils::Transform::Identity() ) );
        break;
    case Gizmo::SCALE:
        m_gizmo.reset( new ScaleGizmo( Utils::Transform::Identity() ) );
        break;
    default:
        break;
    }
}

Model* ModelManager::getSelectedObject() {
    if ( m_selectedmodel == -1 )
        return nullptr;
    else
        return m_models[m_selectedmodel].get();
}

void ModelManager::setSelectedIndex( GLuint index ) {
    m_selectedmodel = glm::max( index, ( GLuint )( m_models.size() - 1 ) );
    m_selectedlight = -1;
}

Light* ModelManager::getSelectedLight() {
    if ( m_selectedlight == -1 )
        return nullptr;
    else
        return m_lights[m_selectedlight].get();
}

void ModelManager::setSelectedLight( GLuint index ) {
    m_selectedlight = glm::max( index, GLuint( m_lights.size() - 1 ) );
    m_selectedmodel = -1;
}
