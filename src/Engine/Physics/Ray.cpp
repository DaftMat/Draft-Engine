/**
 * Created by mathis on 28/02/2020.
 */

#include "Ray.hpp"

Ray::Ray( float x,
          float y,
          float width,
          float height,
          const glm::mat4& projection,
          const glm::mat4& view ) {
    glm::vec4 rayStart_NDC{( x / width - 0.5f ) * 2.f, ( y / height - 0.5f ) * 2.f, -1.f, 1.f};
    glm::vec4 rayEnd_NDC{( x / width - 0.5f ) * 2.f, ( y / height - 0.5f ) * 2.f, 0.f, 1.f};

    glm::mat4 M              = glm::inverse( projection * view );
    glm::vec4 rayStart_world = M * rayStart_NDC;
    rayStart_world /= rayStart_world.w;
    glm::vec4 rayEnd_world = M * rayEnd_NDC;
    rayEnd_world /= rayEnd_world.w;
    glm::vec3 rayDir_world = glm::normalize( rayEnd_world - rayStart_world );

    m_origin    = glm::vec3( rayStart_world );
    m_direction = rayDir_world;
}

bool Ray::intersects( const Utils::Aabb& aabb, float& dist ) const {
    float tMin{0.f};
    float tMax{100000.f};

    for ( int a = 0; a < 3; ++a )
    {
        float invD = 1.0f / m_direction[a];
        float t0   = ( aabb.min()[a] - m_origin[a] ) * invD;
        float t1   = ( aabb.max()[a] - m_origin[a] ) * invD;
        if ( invD < 0.0f )
        {
            float temp = t1;
            t1         = t0;
            t0         = temp;
        }

        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;

        if ( tMax <= tMin ) return false;
    }

    if (tMin > dist)
        return false;
    dist = tMin;
    return true;
}

bool Ray::intersects( const Obb& obb, float& dist ) const {
    float tMin = 0.f;
    float tMax = 100000.f;

    glm::vec3 d = obb.position() - m_origin;
    for ( int i = 0; i < 3; ++i )
    {
        glm::vec3 axis = obb.axis( i );
        float e        = glm::dot( axis, d );
        float f        = glm::dot( m_direction, axis );

        if ( glm::abs( f ) > glm::epsilon<float>() )
        {
            float t1 = ( e + obb.min()[i] ) / f;
            float t2 = ( e + obb.max()[i] ) / f;

            if ( t1 > t2 ) std::swap( t1, t2 );
            if ( t2 < tMax ) tMax = t2;
            if ( t1 > tMin ) tMin = t1;
            if ( tMax < tMin ) return false;
        }
        else if ( -e + obb.min()[i] > 0.f || -e + obb.max()[i] < 0.f )
        { return false; }
    }

    if (tMin > dist)
        return false;
    dist = tMin;
    return true;
}

bool Ray::intersects( const Mesh::Vertex& a,
                      const Mesh::Vertex& b,
                      const Mesh::Vertex& c,
                      float& dist ) const {
    glm::vec3 A = a.Position - c.Position;
    glm::vec3 B = b.Position - c.Position;
    glm::vec3 N = glm::normalize(glm::cross(B, A));
    glm::vec3 T = m_origin - c.Position;

    if (glm::abs(glm::dot(N, m_direction)) < glm::epsilon<float>())
        return false;
    glm::vec3 p = glm::cross(m_direction, B);
    glm::vec3 q = glm::cross(T, A);

    float det = glm::dot(p, A);
    if (glm::abs(det) < glm::epsilon<float>())
        return false;
    float u = (1.f / det) * glm::dot(p, T);
    if (u < 0.f)
        return false;
    float v = (1.f / det) * glm::dot(q, m_direction);
    if (v < 0.f || (u + v) > 1.f)
        return false;

    float t = (1.f / det) * glm::dot(q, B);
    if (t < 0.f || t > dist)
        return false;

    dist = t;
    return true;
}
