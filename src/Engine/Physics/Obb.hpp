/**
 * Created by mathis on 28/02/2020.
 */

#ifndef DAFT_ENGINE_OBB_HPP
#define DAFT_ENGINE_OBB_HPP

#include <opengl_stuff.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Utils/types.hpp>
#include <src/Utils/adapters.hpp>

enum AxisType {
    X = 0,
    Y,
    Z
};

class Obb {
public:
    Obb() : m_aabb(), m_transform { Utils::Transform::Identity() } {}
    Obb(const Utils::Aabb &aabb, const Utils::Transform &transform) : m_aabb { aabb }, m_transform { transform } {}

    /// To directy get the transformed corners
    glm::vec3 corner(int i) const { return toGlm(m_transform * m_aabb.corner(static_cast<Utils::Aabb::CornerType >(i))); }
    glm::vec3 corner(Utils::Aabb::CornerType t) const { return toGlm(m_transform * m_aabb.corner(t)); }

    /// Get the untransformed aabb min/max
    glm::vec3 min() const { return toGlm(m_aabb.min()); }
    glm::vec3 max() const { return toGlm(m_aabb.max()); }

    glm::vec3 position() const;
    glm::vec3 axis(int i) const;
    glm::vec3 axis(AxisType i) const { return axis(int(i)); }

private:
    Utils::Aabb m_aabb;
    Utils::Transform m_transform;
};


#endif //DAFT_ENGINE_OBB_HPP
