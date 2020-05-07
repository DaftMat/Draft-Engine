/**
 * Created by mathis on 05/05/2020.
 */

#include "ParticleSystem.hpp"
#include "Particle.hpp"
#include <3rdPartyLibs/random.hpp>
#include <Engine/Shader.hpp>

using Random = effolkronium::random_static;

ParticleSystem::ParticleSystem() :
    m_normal { 0.f, 1.f, 0.f },
    m_variance { 0.2f, 0.3f, 0.2f },
    m_frequency { 10000.f },
    m_lifetime { 2.f },
    m_gravity { -9.81 },
    m_startSpeed { 6.f },
    m_size { 0.03f },
    m_weight { 0.5f },
    m_clock { 0.f } {}

void ParticleSystem::draw(float dt, const Shader &shader) {
    m_clock += dt;
    /// add new particles
    if (m_clock >= 1.f / m_frequency) {
        for (float i = dt*3.f/4.f ; i < m_clock ; i += 1.f / m_frequency) {
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

    glm::mat4 view = m_view;
    std::sort(m_meshes.begin(), m_meshes.end(), [view](std::unique_ptr<MeshObject> &pa, std::unique_ptr<MeshObject> &pb) {
        glm::vec3 posa {pa->model()[3][0], pa->model()[3][1], pa->model()[3][2]};
        glm::vec3 posb {pb->model()[3][0], pb->model()[3][1], pb->model()[3][2]};
        glm::vec3 viewPos {view[3][0], view[3][1], view[3][2]};
        return glm::length(viewPos - posa) < glm::length(viewPos - posb);
    });

    shader.use();
    shader.setMaterial( material() );

    for ( auto& mesh : m_meshes )
    {
        shader.setMat4( "model", model() * mesh->model() );
        mesh->draw();
    }
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
