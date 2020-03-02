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

    m_modelmanager->addDirLight(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.35f, 0.35f, 0.35f));

    m_creationstate.toCreate = false;
    m_creationstate.type = MODEL;
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
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    checkCreation();
    m_modelmanager->draw(*m_shader, m_camera->viewmatrix(), m_projection, m_camera->position());
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

void Engine::checkCreation() {
    if (m_creationstate.toCreate) {
        m_creationstate.toCreate = false;
        switch (m_creationstate.type) {
            case UV_SPHERE:
                m_modelmanager->addUVSphere();
                break;
            case ICO_SPHERE:
                m_modelmanager->addIcoSphere();
                break;
            case CUBE_SPHERE:
                m_modelmanager->addCubeSphere();
                break;
            case MODEL:
                break;
        }
        m_modelmanager->setSelectedIndex(m_modelmanager->getSize()-1);
    }
}

void Engine::setModelParams(ModelType type, const ModelParam &params) {
    switch (type) {
    case UV_SPHERE:
        m_modelmanager->setUVSphereParams(params.uv_sphere.meridians, params.uv_sphere.parallels);
        break;
    case ICO_SPHERE:
        m_modelmanager->setIcoSphereParams(params.ico_sphere.subdivisions);
        break;
    case CUBE_SPHERE:
        m_modelmanager->setCubeSphereParams(params.cube_sphere.resolution);
        break;
    case MODEL:
        break;
    }
}
