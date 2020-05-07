/**
 * Created by mathis on 05/05/2020.
 */

#include "ParticleSystem.hpp"
#include "Particle.hpp"
#include <3rdPartyLibs/random.hpp>

using Random = effolkronium::random_static;

ParticleSystem::ParticleSystem() :
    m_normal { 0.f, 1.f, 0.f },
    m_variance { 0.3f, 0.f, 0.3f },
    m_frequency { 200.f },
    m_lifetime { 5.f },
    m_gravity { -9.81 },
    m_startSpeed { 6.f },
    m_size { .1f },
    m_weight { 0.5f },
    m_clock { 0.f } {}

void ParticleSystem::draw(float dt, const Shader &shader) {
    m_clock += dt;
    /// add new particles
    if (m_clock >= 1.f / m_frequency) {
        for (float i = dt ; i <= m_clock ; i += 1.f / m_frequency) {
            m_meshes.emplace_back(new Particle(glm::vec3 {0.f},
                    m_startSpeed * glm::normalize(glm::vec3 {
                Random::get(m_normal.x - m_variance.x, m_normal.x + m_variance.x),
                Random::get(m_normal.y - m_variance.y, m_normal.y + m_variance.y),
                Random::get(m_normal.z - m_variance.z, m_normal.z + m_variance.z)
            }), m_size, m_view));
        }
        m_clock = 0.f;
    }

    float maxLifeTime = m_lifetime;
    m_meshes.erase(std::remove_if(m_meshes.begin(), m_meshes.end(),
            [maxLifeTime](const std::unique_ptr<MeshObject> &p) { return p->getSetting() >= maxLifeTime; }), m_meshes.end());

    for (auto &particle: m_meshes) {
        particle->update(dt);
    }

    if (!m_meshes.empty())
        Model::draw(dt, shader);
}

Model::ModelParam ParticleSystem::getParams() const {
    Model::ModelParam params {};
    params.particlesys.normal = m_normal;
    params.particlesys.variance = m_variance;
    params.particlesys.frequency = m_frequency;
    params.particlesys.lifetime = m_lifetime;
    params.particlesys.gravity = m_gravity;
    params.particlesys.startSpeed = m_startSpeed;
    params.particlesys.weight = m_weight;
    params.particlesys.view = m_view;
    return params;
}

void ParticleSystem::editModel(const Model::ModelParam &params) {
    m_normal = params.particlesys.normal;
    m_variance = params.particlesys.variance;
    m_frequency = params.particlesys.frequency;
    m_lifetime = params.particlesys.lifetime;
    m_gravity = params.particlesys.gravity;
    m_startSpeed = params.particlesys.startSpeed;
    m_weight = params.particlesys.weight;
    m_view = params.particlesys.view;
}
