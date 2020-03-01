/**
 * Created by mathis on 12/02/2020.
 */

#include <src/Engine/Geometry/Primitives/UVSphere.hpp>
#include <src/Engine/Geometry/Primitives/IcoSphere.hpp>
#include <src/Engine/Geometry/Primitives/CubeSphere.hpp>
#include <src/Engine/Geometry/Lights/DirLight.hpp>
#include <src/Engine/Geometry/Lights/SpotLight.hpp>
#include "ModelManager.hpp"

void ModelManager::draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &viewPos) {
    drawGrid(projection, view);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_gizmo->scale(glm::length(viewPos) / 3.f);
    m_gizmo->draw(projection, view);
    if (m_models.empty())   return;

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
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_colorshader->use();
        if (i == m_selectedmodel) {
            m_colorshader->setVec3("color", glm::vec3(0.f, 1.f, 1.f));
            m_models[i]->draw(*m_colorshader);
        } else if (m_wireframe) {
            m_colorshader->setVec3("color", glm::vec3(0.f, 0.f, 0.f));
            m_models[i]->draw(*m_colorshader);
        }
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

void ModelManager::setUVSphereParams(GLuint meridians, GLuint parallels) {
    ModelParam new_params = m_models[m_selectedmodel]->getParams();
    new_params.uv_sphere.meridians = meridians;
    new_params.uv_sphere.parallels = parallels;
    m_models[m_selectedmodel]->editModel(new_params);
    m_toReset.insert(m_selectedmodel);
}

void ModelManager::setIcoSphereParams(GLuint subdivisions) {
    ModelParam new_params = m_models[m_selectedmodel]->getParams();
    new_params.ico_sphere.subdivisions = subdivisions;
    m_models[m_selectedmodel]->editModel(new_params);
    m_toReset.insert(m_selectedmodel);
}

void ModelManager::setCubeSphereParams(GLuint resolution) {
    ModelParam new_params = m_models[m_selectedmodel]->getParams();
    new_params.cube_sphere.resolution = resolution;
    m_models[m_selectedmodel]->editModel(new_params);
    m_toReset.insert(m_selectedmodel);
}

bool ModelManager::keyboard(unsigned char key) {
    switch (key) {
        case 'o':
            switch_selection();
            return true;
        case 'd':
            deleteModel();
            return true;
        default:
            return false;
    }
}

void ModelManager::switch_selection() {
    if (!m_models.empty())
        m_selectedmodel = (m_selectedmodel + 1) % m_models.size();
    else
        m_selectedmodel = -1;
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
            switch_selection();
            return;
        }
    }
}

void ModelManager::mouse_click(const Ray &ray) {
    float dist = 100000.f;
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
        found = false;
    }
    if (found)  return;
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
}
