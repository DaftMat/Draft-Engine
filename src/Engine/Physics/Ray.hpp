//
// Created by mathis on 28/02/2020.
//

#ifndef DAFT_ENGINE_RAY_HPP
#define DAFT_ENGINE_RAY_HPP

#include <opengl_stuff.h>

#include "Obb.hpp"
#include <src/Utils/types.hpp>
#include <src/Engine/Geometry/Mesh.hpp>

/**
 *  A ray that can be found in raycast/raytrace techniques.
 */
class Ray
{
  public:
    Ray() = default;

    /** Constructor.
     *
     * @param origin - origin position of the ray.
     * @param direction - direction of the ray.
     */
    Ray( const glm::vec3& origin, const glm::vec3& direction ) :
        m_origin{origin},
        m_direction{direction} {}

    /** Screen Constructor.
     *  Creates a Ray from a position on an OpenGL screen.
     * @param x - x position on screen.
     * @param y - y position on screen.
     * @param width - width of the screen.
     * @param height - height - of the screen.
     * @param projection - projection matrix of the camera.
     * @param view - view matrix of the camera.
     */
    Ray( float x,
         float y,
         float width,
         float height,
         const glm::mat4& projection,
         const glm::mat4& view );

    /** ray - Aligned Axis Bounding Box intersection.
     *
     * @param aabb - AABB to test.
     * @param dist - output - distance between AABB and ray if intersected, unchanged otherwise.
     * @return true if intersected, false otherwise.
     */
    bool intersects( const Utils::Aabb& aabb, float& dist ) const;

    /** ray - Oriented Bounding Box intersection.
     *
     * @param obb - OBB to test.
     * @param dist - output - distance between OBB and ray if intersected, unchanged otherwise.
     * @return true if intersected, false otherwise.
     */
    bool intersects( const Obb& obb, float& dist ) const;

    /** ray - Triangle intersection.
     *
     * @param a - triangle's 1st vertex.
     * @param b - triangle's 2nd vertex.
     * @param c - triangle's 3rd vertex.
     * @param dist - output - distance between triangle and ray if instersected, unchanged
     * otherwise.
     * @return true if intersected, false otherwise.
     */
    bool
    intersects( const Mesh::Vertex& a, const Mesh::Vertex& b, const Mesh::Vertex& c, float& dist ) const;

    /** "Position" of the ray's end.
     *
     * @param dist - dist from ray's origin
     * @return O + dist * D
     */
    glm::vec3 position( float dist ) const { return m_origin + dist * m_direction; }

    /** Origin.
     *
     * @return position when dist is 0.
     */
     const glm::vec3& origin() const { return m_origin; }

     /** Direction.
      *
      * @return direction of the ray.
      */
     const glm::vec3& direction() const { return m_direction; }

  private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

#endif // DAFT_ENGINE_RAY_HPP
