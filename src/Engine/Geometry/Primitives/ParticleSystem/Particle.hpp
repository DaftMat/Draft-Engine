/**
 * Created by mathis on 06/05/2020.
 */

#ifndef DAFT_ENGINE_PARTICLE_HPP
#define DAFT_ENGINE_PARTICLE_HPP

#include <src/Engine/Geometry/MeshObject.hpp>
#include <src/Engine/Cameras/camera.h>

/** class for particles.
 * billboard.
 */
class Particle : public MeshObject {
public:

    Particle(const glm::vec3 &position, const glm::vec3 &speed, float size, float gravity, const glm::mat4 &view)
    : m_position { position }, m_speed { speed }, m_size { size }, m_gravity {gravity} { buildMesh(view); }

    glm::mat4 model() const override { return glm::translate(glm::mat4(1.f), m_position); }
    void update(float dt) override;
    void draw(const Shader &shader) override;
    float getSetting() const override { return m_lifetime; }
private:

    void buildMesh(const glm::mat4 &view);

    glm::vec3 m_position;
    glm::vec3 m_speed;
    float m_size;
    float m_gravity;
    float m_lifetime { 0.f };   ///< current lifetime
};


#endif //DAFT_ENGINE_PARTICLE_HPP
