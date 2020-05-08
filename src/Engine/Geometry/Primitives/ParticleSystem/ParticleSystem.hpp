/**
 * Created by mathis on 05/05/2020.
 */

#ifndef DAFT_ENGINE_PARTICLESYSTEM_HPP
#define DAFT_ENGINE_PARTICLESYSTEM_HPP


#include <src/Engine/Geometry/Model.hpp>
#include <src/Engine/Cameras/camera.h>


class ParticleSystem : public Model {
public:

    /** Constructor.
     *
     */
    ParticleSystem();

    void draw(float dt, const Shader &shader) override;

    Model::ModelType getType() const override { return PARTICLESYS; }
    Model::ModelParam getParams() const override;
    void editModel(const Model::ModelParam &params) override;

private:
    glm::vec3 m_normal;     ///< center direction of the particles (velocity)
    glm::vec3 m_variance;   ///< possible difference from the center
    float m_velocity;
    float m_frequency;  ///< number of particles per seconds
    float m_lifetime;   ///< total life time of a particle
    float m_gravity;    ///< gravity impact on particles.
    float m_size;
    float m_clock; ///< add dt to it each update and release a particle when it gets to 1/freq

    glm::mat4 m_view;
};


#endif //DAFT_ENGINE_PARTICLESYSTEM_HPP
