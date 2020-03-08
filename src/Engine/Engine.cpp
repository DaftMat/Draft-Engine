//
// Created by daftmat on 27/01/20.
//

#include <QtWidgets/QMainWindow>
#include <src/Engine/Physics/Ray.hpp>
#include "Engine.hpp"

Engine::Engine(int width, int height) :
        m_width { width },
        m_height { height },
        m_modelmanager { new ModelManager() },
        m_activeshader { 0 },
        m_shader { nullptr },
        m_activecamera { 0 },
        m_camera { nullptr } {
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);
    glEnable(GL_MULTISAMPLE);

    /// Setup shaders
    m_shaderselector.emplace_back( []()->Shader*{ return new Shader("shaders/blinnphong.vert.glsl", "shaders/blinnphong.frag.glsl"); } );
    m_shaderselector.emplace_back( []()->Shader*{ return new Shader("shaders/error.vert.glsl", "shaders/error.frag.glsl"); } );
    m_shaderselector.emplace_back( []()->Shader*{ return new Shader("shaders/default.vert.glsl", "shaders/default.frag.glsl"); } );
    m_shader.reset(m_shaderselector[m_activeshader]());

    /// Setup cameras
    m_cameraselector.emplace_back( []()->Camera*{ return new EulerCamera(glm::vec3(0.f, 0.f, 6.f)); } );
    m_cameraselector.emplace_back( []()->Camera*{ return new TrackballCamera(glm::vec3(0.f, 0.f, 6.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f)); } );
    m_camera.reset(m_cameraselector[m_activecamera]());
    m_camera->setviewport(glm::vec4(0.f, 0.f, m_width, m_height));

    m_projection = glm::perspective(m_camera->zoom(), float(m_width) / m_height, 0.1f, 100.0f);

    //m_modelmanager->addLight(DIR_LIGHT);

    m_creationstate.toCreate = false;
    m_creationstate.type = MODEL;

    m_lightcreationstate.toCreate = false;
}

Engine::~Engine() {
    m_shader.reset();
    m_camera.reset();
    m_modelmanager.reset();
}


void Engine::resize(int width, int height) {
    m_width = width;
    m_height = height;
    m_camera->setviewport(glm::vec4(0.f, 0.f, m_width, m_height));
    m_projection = glm::perspective(m_camera->zoom(), float(m_width) / m_height, 0.1f, 100.0f);
}

void Engine::draw() {
    checkCreation();
    m_modelmanager->draw(*m_shader, m_camera->viewmatrix(), m_projection, m_camera->position(), m_camera->front());
}


void Engine::mouseclick(int button, float xpos, float ypos) {
    m_button = button;
    m_mousex = xpos;
    m_mousey = ypos;
    m_clicked_object = m_modelmanager->mouse_click(Ray(xpos, m_height-ypos, m_width, m_height, m_projection, m_camera->viewmatrix()), xpos, m_height-ypos);
    m_camera->processmouseclick(m_button, xpos, ypos);
}

void Engine::mouserelease() {
    m_clicked_object = false;
    m_modelmanager->mouserelease();
}

void Engine::mousemove(float xpos, float ypos) {
    if (!m_clicked_object)
        m_camera->processmousemovement(m_button, xpos, ypos, true);
    else
        m_modelmanager->mouse_move(xpos, m_height-ypos, m_projection, m_camera->viewmatrix());
}

void Engine::keyboardmove(int key, double time) {
    m_camera->processkeyboard(Camera_Movement(key), time);
}

bool Engine::keyboard(unsigned char k) {
    switch(k) {
        case 'p':
            m_activecamera = (m_activecamera+1)%2;
            m_camera.reset(m_cameraselector[m_activecamera]());
            m_camera->setviewport(glm::vec4(0.f, 0.f, m_width, m_height));
            return true;
        case 's':
            m_activeshader = (m_activeshader+1)%(unsigned)m_shaderselector.size();
            m_shader.reset(m_shaderselector[m_activeshader]());
            return true;
        default:
            return m_modelmanager->keyboard(k);
    }
}

void Engine::shaderChanged(GLuint selected) {
    m_activeshader = selected;
    if (m_activeshader >= m_shaderselector.size())
        m_activeshader = m_shaderselector.size() - 1;
    m_shader.reset(m_shaderselector[m_activeshader]());
}

void Engine::addModel(ModelType type) {
    m_creationstate.toCreate = true;
    m_creationstate.type = type;
}

void Engine::addLight(LightType type) {
    m_lightcreationstate.toCreate = true;
    m_lightcreationstate.type = type;
}

void Engine::checkCreation() {
    if (m_creationstate.toCreate) {
        m_creationstate.toCreate = false;
        m_modelmanager->addObject(m_creationstate.type);
        m_modelmanager->setSelectedIndex(m_modelmanager->getSize()-1);
    }
    if (m_lightcreationstate.toCreate) {
        m_lightcreationstate.toCreate = false;
        m_modelmanager->addLight(m_lightcreationstate.type);
        m_modelmanager->setSelectedLight(m_modelmanager->getNumLights()-1);
    }
}

void Engine::setModelParams(const ModelParam &params) {
    m_modelmanager->setObjectParams(params);
}
