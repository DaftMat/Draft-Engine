/**
 * Created by mathis on 01/03/2020.
 */

#include "Gizmo.hpp"

#include <Utils/adapters.hpp>
void Gizmo::draw(const glm::mat4 &projection, const glm::mat4 &view) {
    m_shader->use();
    m_shader->setMat4("projection", projection);
    m_shader->setMat4("view", view);
    m_shader->setVec3("color", glm::vec3(1.f, 0.f, 0.f));
    if (m_selected == XSELEC)
        m_shader->setVec3("color", glm::vec3(0.f, 1.f, 1.f));
    m_Xmodel->draw(*m_shader);
    m_shader->setVec3("color", glm::vec3(0.f, 1.f, 0.f));
    if (m_selected == YSELEC)
        m_shader->setVec3("color", glm::vec3(0.f, 1.f, 1.f));
    m_Ymodel->draw(*m_shader);
    m_shader->setVec3("color", glm::vec3(0.f, 0.f, 1.f));
    if (m_selected == ZSELEC)
        m_shader->setVec3("color", glm::vec3(0.f, 1.f, 1.f));
    m_Zmodel->draw(*m_shader);
}