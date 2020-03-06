/**
 * Created by mathis on 12/02/2020.
 */

#include <Engine/Geometry/Primitives/UVSphere.hpp>
#include <Engine/Geometry/Primitives/IcoSphere.hpp>
#include <Engine/Geometry/Primitives/CubeSphere.hpp>
#include <Engine/Geometry/Primitives/Cube.hpp>
#include <Engine/Geometry/Lights/DirLight.hpp>
#include <Engine/Geometry/Lights/SpotLight.hpp>
#include "ModelManager.hpp"

void ModelManager::draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &viewPos) {
    if (m_edition)
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    else
        glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_edition)
        drawGrid(projection, view);

    if (m_models.empty())   return;

    if ((m_selectedmodel > -1 || m_selectedlight > -1) && m_edition) {
        updateGizmo(viewPos);
        glDepthRange(0.0, 0.01);
        m_gizmo->draw(projection, view);
        glDepthRange(0.01, 1.0);
    }

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

    for(GLuint i = 0 ; i < m_models.size() ; ++i) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_models[i]->draw(shader);
        if (m_edition) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            m_colorshader->use();
            if (i == m_selectedmodel) {
                m_colorshader->setVec3("color", glm::vec3(0.f, 1.f, 1.f));
                m_models[i]->draw(*m_colorshader);
            } else if (m_wireframe) {
                m_colorshader->setVec3("color", glm::vec3(0.f, 0.f, 0.f));
                m_models[i]->draw(*m_colorshader);
            }
            //draw lights "models"
        }
    }
    glDepthRange(0.0, 1.0);

    shader.clearLights();
}

void ModelManager::addLight(LightType type) {
    switch (type) {
        case POINT_LIGHT:
            m_lights.emplace_back(new PointLight());
            break;
        case DIR_LIGHT:
            m_lights.emplace_back(new DirLight());
            break;
        case SPOT_LIGHT:
            m_lights.emplace_back(new SpotLight());
            break;
        default:
            break;
    }
}

void ModelManager::addObject(ModelType type) {
    switch (type) {
        case UV_SPHERE:
            m_models.emplace_back(new UVSphere());
            break;
        case ICO_SPHERE:
            m_models.emplace_back(new IcoSphere());
            break;
        case CUBE_SPHERE:
            m_models.emplace_back(new CubeSphere());
            break;
        case CUBE:
            m_models.emplace_back(new Cube());
            break;
        default:
            break;
    }
}

void ModelManager::setObjectParams(const ModelParam &params) {
    m_models[m_selectedmodel]->editModel(params);
    m_toReset.insert(m_selectedmodel);
}

bool ModelManager::keyboard(unsigned char key) {
    switch (key) {
        case 'd':
            deleteModel();
            return true;
        default:
            return false;
    }
}

void ModelManager::makeGrid(int size) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLuint index;
    Vertex vertex {};
    glm::vec3 normal { 0.f, 1.f, 0.f };
    for (int i = 0 ; i < size ; ++i) {
        for (int j = 0 ; j < size ; ++j) {
            index = (GLuint)(i + j * size);
            vertex.Normal = normal;
            vertex.Position = glm::vec3((float)(i-(size/2)), 0.f, (float)(j-(size/2)));
            vertices.push_back(vertex);

            if (i < size && j < size && (i != size/2 || j != size/2)) {
                if (i < size - 1) {
                    indices.push_back(index);
                    indices.push_back(index + 1);
                }
                if (j < size - 1) {
                    indices.push_back(index);
                    indices.push_back(index + size);
                }
            }
        }
    }

    m_grid.reset(new Mesh(vertices, indices, true));
}

void ModelManager::drawGrid(const glm::mat4 &projection, const glm::mat4 &view) {
    m_colorshader->use();
    m_colorshader->setMat4("projection", projection);
    m_colorshader->setMat4("view", view);
    m_colorshader->setMat4("model", glm::mat4());
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /// Draw grid
    m_colorshader->setVec3("color", glm::vec3(0.25f, 0.25f, 0.25f));
    m_grid->draw();
    /// Draw unit arrows
    for (int i = 0 ; i < 3 ; ++i) {
        m_colorshader->setMat4("model", glm::mat4());
        m_colorshader->setVec3("color", dirs()[i]);
        m_unitarrows[i]->draw();
    }

}

void ModelManager::makeUnitArrows() {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices { 0, 1 };
    Vertex vertex {};
    Vertex center {};
    for (int i = 0 ; i < 3 ; ++i) {
        vertices.push_back(center);
        vertex.Position = dirs()[i];
        vertices.push_back(vertex);
        m_unitarrows.emplace_back(new Mesh(vertices, indices, true));
        vertices.clear();
    }

}

void ModelManager::deleteModel() {
    if (m_selectedmodel < 0) return;
    for (auto it = m_models.begin() ; it != m_models.end() ; ++it) {
        if (*it == m_models[m_selectedmodel]) {
            m_models.erase(it);
            return;
        }
    }
}

bool ModelManager::mouse_click(const Ray &ray, float xpos, float ypos) {
    float dist = 100000.f;
    if (m_selectedmodel != -1) {
        bool found = false;
        if (ray.intersects(m_gizmo->getXobb(), dist)) {
            m_gizmo->setSelected(XSELEC);
            found = true;
        } else if (ray.intersects(m_gizmo->getYobb(), dist)) {
            m_gizmo->setSelected(YSELEC);
            found = true;
        } else if (ray.intersects(m_gizmo->getZobb(), dist)) {
            m_gizmo->setSelected(ZSELEC);
            found = true;
        } else {
            m_gizmo->setSelected(NONE);
        }
        if (found) {
            m_gizmo->clicked(xpos, ypos);
            return found;
        }
    }
    bool found;
    found = false;
    for (int i = 0 ; i < m_models.size() ; ++i) {
        float temp;
        if (ray.intersects(m_models[i]->obb(), temp)) {
            found = true;
            if (temp < dist) {
                dist = temp;
                m_selectedmodel = i;
            }
        }
    }
    if (!found) m_selectedmodel = -1;
    else        m_selectedlight = -1;
    bool found_light = false;
    for (int i = 0 ; i < m_lights.size() ; ++i) {
        float temp;
        if (ray.intersects(m_lights[i]->model().obb(), temp)) {
            if (temp < dist) {
                found_light = true;
                dist = temp;
                m_selectedlight = i;
            }
        }
    }
    if (!found_light) m_selectedlight = -1;
    else m_selectedmodel = -1;
    return found || found_light;
}

void ModelManager::updateGizmo(const glm::vec3 &viewPos) {
    if (m_gizmoType != m_gizmo->type())
        switchGizmo();
    if (m_selectedmodel > -1 && m_selectedmodel < m_models.size()) {
        m_gizmo->setTransform(*m_models[m_selectedmodel]);
        m_gizmo->scale(glm::length(viewPos - m_models[m_selectedmodel]->getPosition()) / 3.f);
    }
}

void ModelManager::mouse_move(float xpos, float ypos, const glm::mat4 &projection, const glm::mat4 &view) {
    if (m_gizmo->isSelected())
        m_gizmo->move(xpos, ypos, *m_models[m_selectedmodel], projection, view);
}

void ModelManager::mouserelease() {
    m_gizmo->setSelected(NONE);
}

void ModelManager::switchGizmo() {
    switch(m_gizmoType) {
        case TRANSLATE:
            m_gizmo.reset(new TranslateGizmo(Utils::Transform::Identity()));
            break;
        case SCALE:
            m_gizmo.reset(new ScaleGizmo(Utils::Transform::Identity()));
            break;
        default:
            break;
    }
}

void ModelManager::setLightParams(const LightParam &params, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) {
    m_lights[m_selectedlight]->editLight(params);
    m_lights[m_selectedlight]->ambient() = ambient;
    m_lights[m_selectedlight]->diffuse() = diffuse;
    m_lights[m_selectedlight]->specular() = specular;
}
