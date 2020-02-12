/**
 * Created by mathis on 12/02/2020.
 */

#include <src/hello_sphere/Geometry/Primitives/UVSphere.hpp>
#include <src/hello_sphere/Geometry/Primitives/IcoSphere.hpp>
#include "ModelManager.hpp"

void ModelManager::draw(const Shader &shader) {
    for (const auto &ind : m_toReset)
        m_models[ind]->reset();
    m_toReset.clear();

    for (const auto &model : m_models)
        model->draw(shader);
}

void ModelManager::addUVSphere(GLuint meridians, GLuint parallels) {
    m_models.emplace_back(new UVSphere(meridians, parallels));
}

void ModelManager::addIcoSphere(GLuint subdivisions) {
    m_models.emplace_back(new IcoSphere(subdivisions));
}

ModelParam ModelManager::add_uvsphere_params(GLuint meridians, GLuint parallels) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.uv_sphere.meridians += meridians;
    result.uv_sphere.parallels += parallels;
    return result;
}

ModelParam ModelManager::add_icosphere_params(GLuint subdivisions) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.ico_sphere.subdivisions += subdivisions;
    return result;
}

ModelParam ModelManager::sub_uvsphere_params(GLuint meridians, GLuint parallels) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.uv_sphere.meridians -= meridians;
    result.uv_sphere.parallels -= parallels;
    return result;
}

ModelParam ModelManager::sub_icosphere_params(GLuint subdivisions) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.ico_sphere.subdivisions -= subdivisions;
    return result;
}


bool ModelManager::keyboard(unsigned char key) {
    switch (key) {
        case 'o':
            switch_selection();
            return true;
        case '+': {
            switch (m_models[m_selectedmodel]->getType()) {
                case UV_SPHERE:
                    m_models[m_selectedmodel]->editModel(add_uvsphere_params());
                    m_toReset.insert(m_selectedmodel);
                    return true;
                case ICO_SPHERE:
                    m_models[m_selectedmodel]->editModel(add_icosphere_params());
                    m_toReset.insert(m_selectedmodel);
                    return true;
                default:
                    return false;
            }
        } case '-': {
            switch (m_models[m_selectedmodel]->getType()) {
                case UV_SPHERE:
                    m_models[m_selectedmodel]->editModel(sub_uvsphere_params());
                    m_toReset.insert(m_selectedmodel);
                    return true;
                case ICO_SPHERE:
                    m_models[m_selectedmodel]->editModel(sub_icosphere_params());
                    m_toReset.insert(m_selectedmodel);
                    return true;
                default:
                    return false;
            }
        } default:
            return false;
    }
}

void ModelManager::switch_selection() {
    if (!m_models.empty())
        m_selectedmodel = (m_selectedmodel + 1) % m_models.size();
}
