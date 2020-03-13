//
// Created by mathis on 28/02/2020.
//

#ifndef DAFT_ENGINE_OBB_HPP
#define DAFT_ENGINE_OBB_HPP

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <opengl_stuff.h>

#include <Utils/types.hpp>
#include <src/Utils/adapters.hpp>

/**
 * Oriented Bounding Box.
 * A Bounding Box that rotate with the affiliated object
 */
class Obb
{
  public:
    /** type of axis for obb planes */
    enum AxisType { X = 0, Y, Z };

    /** Default constructor.
     * creates an untransformed obb.
     */
    Obb() : m_aabb(), m_transform{Utils::Transform::Identity()} {}

    /** Constructor.
     *
     * @param aabb - AABB of the affiliated object.
     * @param transform - transformation of the object.
     */
    Obb( const Utils::Aabb& aabb, const Utils::Transform& transform ) :
        m_aabb{aabb},
        m_transform{transform} {}

    /** untransformed aabb min getter.
     *
     * @return aabb min.
     */
    glm::vec3 min() const { return toGlm( m_aabb.min() ); }

    /** untransformed aabb max getter.
     *
     * @return aabb max.
     */
    glm::vec3 max() const { return toGlm( m_aabb.max() ); }

    /** obb's position getter.
     *
     * @return position of OBB.
     */
    glm::vec3 position() const;

    /** util function for the axis of obb's planes.
     *
     * @param i - index of the axis (0, 1, 2)
     * @return
     */
    glm::vec3 axis( int i ) const;
    glm::vec3 axis( AxisType i ) const { return axis( int( i ) ); }

  private:
    Utils::Aabb m_aabb;
    Utils::Transform m_transform;
};

#endif // DAFT_ENGINE_OBB_HPP
