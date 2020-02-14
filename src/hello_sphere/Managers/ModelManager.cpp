/**
 * Created by mathis on 12/02/2020.
 */

#include <src/hello_sphere/Geometry/Primitives/UVSphere.hpp>
#include <src/hello_sphere/Geometry/Primitives/IcoSphere.hpp>
#include <src/hello_sphere/Geometry/Primitives/CubeSphere.hpp>
#include <src/hello_sphere/Geometry/Lights/DirLight.hpp>
#include <src/hello_sphere/Geometry/Lights/SpotLight.hpp>
#include "ModelManager.hpp"

void ModelManager::draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &viewPos) {
    for (const auto &ind : m_toReset)
        m_models[ind]->reset();
    m_toReset.clear();

    shader.use();
    shader.setVec3("viewPos", viewPos);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    for (const auto &light : m_lights) {
        shader.addLight(light.get());
    }
    m_yellowshader->use();
    m_yellowshader->setMat4("view", view);
    m_yellowshader->setMat4("projection", projection);
    if (m_wireframe) {
        m_blackshader->use();
        m_blackshader->setMat4("view", view);
        m_blackshader->setMat4("projection", projection);
    }

    for(GLuint i = 0 ; i < m_models.size() ; ++i) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_models[i]->draw(shader);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (i == m_selectedmodel)
            m_models[i]->draw(*m_yellowshader);
        else if (m_wireframe)
            m_models[i]->draw(*m_blackshader);
    }

    shader.clearLights();
}

void ModelManager::addPointLight(const glm::vec3 &pos, GLfloat constant, GLfloat linear, GLfloat quadratic,
                                 const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) {
    m_lights.emplace_back(new PointLight(pos, constant, linear, quadratic, ambient, diffuse, specular));
}

void ModelManager::addDirLight(const glm::vec3 &dir, const glm::vec3 &ambient, const glm::vec3 &diffuse,
                               const glm::vec3 &specular) {
    m_lights.emplace_back(new DirLight(dir, ambient, diffuse, specular));
}

void ModelManager::addSpotLight(const glm::vec3 &pos, const glm::vec3 &dir, float innerCutoff, float outerCutoff,
                                float constant, float linear, float quadratic, const glm::vec3 &ambient,
                                const glm::vec3 &diffuse, const glm::vec3 &specular) {
    m_lights.emplace_back(new SpotLight(pos, dir, innerCutoff, outerCutoff,
            constant, linear, quadratic, ambient, diffuse, specular));
}

void ModelManager::addUVSphere(GLuint meridians, GLuint parallels) {
    m_models.emplace_back(new UVSphere(meridians, parallels));
}

void ModelManager::addIcoSphere(GLuint subdivisions) {
    m_models.emplace_back(new IcoSphere(subdivisions));
}

void ModelManager::addCubeSphere(GLuint resolution) {
    m_models.emplace_back(new CubeSphere(resolution));
}

ModelParam ModelManager::add_uvsphere_params(GLuint meridians, GLuint parallels) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.uv_sphere.meridians += meridians;
    result.uv_sphere.parallels += parallels;
    return result;
}

ModelParam ModelManager::sub_uvsphere_params(GLuint meridians, GLuint parallels) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.uv_sphere.meridians -= meridians;
    result.uv_sphere.parallels -= parallels;
    return result;
}

ModelParam ModelManager::add_icosphere_params(GLuint subdivisions) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.ico_sphere.subdivisions += subdivisions;
    return result;
}

ModelParam ModelManager::sub_icosphere_params(GLuint subdivisions) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.ico_sphere.subdivisions -= subdivisions;
    return result;
}

ModelParam ModelManager::add_cubesphere_params(GLuint resolution) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.cube_sphere.resolution += resolution;
    return result;
}

ModelParam ModelManager::sub_cubesphere_params(GLuint resolution) {
    ModelParam result = m_models[m_selectedmodel]->getParams();
    result.cube_sphere.resolution -= resolution;
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
                case CUBE_SPHERE:
                    m_models[m_selectedmodel]->editModel(add_cubesphere_params());
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
                case CUBE_SPHERE:
                    m_models[m_selectedmodel]->editModel(sub_cubesphere_params());
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
