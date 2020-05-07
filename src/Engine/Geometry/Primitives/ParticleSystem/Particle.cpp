/**
 * Created by mathis on 06/05/2020.
 */

#include "Particle.hpp"
#include <Engine/Shader.hpp>

float Particle::gravity = -9.81f;

void Particle::buildMesh(const glm::mat4 &view) {
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices { 0, 1, 2, 0, 2, 3 };
    std::vector<glm::vec2> square {
        glm::vec2 {-1.f, 1.f} * m_size,
        glm::vec2 {1.f, 1.f} * m_size,
        glm::vec2 {1.f, -1.f} * m_size,
        glm::vec2 {-1.f, -1.f} * m_size
    };
    std::vector<glm::vec2> tex{
        {0.f, 0.f},
        {1.f, 0.f},
        {1.f, 1.f},
        {0.f, 1.f}
    };

    Mesh::Vertex vertex;
    vertex.Normal = - glm::vec3 { view[0][2], view[1][2], view[2][2] };
    glm::vec3 camera_right  { view[0][0], view[1][0], view[2][0] };
    glm::vec3 camera_up     { view[0][1], view[1][1], view[2][1] };

    for (int i = 0 ; i < 4 ; ++i) {
        vertex.TexCoords = tex[i];
        vertex.Position = m_position + camera_right * square[i].x + camera_up * square[i].y;
        vertices.push_back(vertex);
    }

    m_mesh = std::unique_ptr<Mesh>(new Mesh(vertices, indices));
}

void Particle::update(float dt) {
    m_lifetime += dt;
    m_speed += dt * glm::vec3 { 0.f, gravity, 0.f };
    m_position += m_speed * dt;
}

void Particle::draw(const Shader &shader) {
    shader.use();
    shader.setVec3("particlePosition", {0.f, 0.f, 0.f});
    shader.setFloat("particleRadius", m_size);
    m_mesh->draw();
}
