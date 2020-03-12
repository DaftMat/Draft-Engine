//
// Created by mathis on 28/02/2020.
//

#ifndef DAFT_ENGINE_RAY_HPP
#define DAFT_ENGINE_RAY_HPP

#include "Obb.hpp"
#include <opengl_stuff.h>
#include <src/Utils/types.hpp>

class Ray
{
  public:
    Ray( const glm::vec3& origin, const glm::vec3& direction ) :
        m_origin{origin}, m_direction{direction} {}
    Ray( float x,
         float y,
         float width,
         float height,
         const glm::mat4& projection,
         const glm::mat4& view );

    bool intersects( const Utils::Aabb& aabb, float& dist ) const;
    bool intersects( const Obb& obb, float& dist ) const;
    /// Triangle intersection
    bool
    intersects( const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& dist ) const;

    glm::vec3 position( float dist ) { return m_origin + dist * m_direction; }

  private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

#endif // DAFT_ENGINE_RAY_HPP
