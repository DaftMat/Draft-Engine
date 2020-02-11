//
// Created by daftmat on 27/01/20.
//

#include <src/hello_sphere/Geometry/Primitives/IcoSphere.hpp>
#include "hellospheres.hpp"

Hellospheres::Hellospheres(int width, int height)
        : OpenGLDemo(width, height),
        m_Icosphere { new IcoSphere() },
        m_UVsphere { new UVSphere() },
        m_activeshader { 0 },
        m_shader { nullptr },
        m_activecamera { 0 },
        m_camera { nullptr },
        m_resetMeshes { false } {
    /// Setup shaders
    m_shaderselector.emplace_back( []()->Shader*{ return new Shader("shaders/default.vert.glsl", "shaders/default.frag.glsl"); } );
    m_shaderselector.emplace_back( []()->Shader*{ return new Shader("shaders/error.vert.glsl", "shaders/error.frag.glsl"); } );
    m_shader.reset(m_shaderselector[m_activeshader]());

    /// Setup cameras
    m_cameraselector.emplace_back( []()->Camera*{ return new EulerCamera(glm::vec3(0.f, 0.f, 6.f)); } );
    m_cameraselector.emplace_back( []()->Camera*{ return new TrackballCamera(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f)); } );
    m_camera.reset(m_cameraselector[m_activecamera]());
    m_camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));

    m_view = m_camera->viewmatrix();
    m_projection = glm::perspective(m_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);

    m_UVsphere->transform(glm::translate(m_UVsphere->model(), glm::vec3(-2.5f, 0.f, 0.f)));
    m_Icosphere->transform(glm::translate(m_Icosphere->model(), glm::vec3(0.f, 0.f, 0.f)));

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);
}

Hellospheres::~Hellospheres() {
    m_shader.reset();
    m_camera.reset();
}


void Hellospheres::resize(int width, int height) {
    OpenGLDemo::resize(width, height);
    m_camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    m_projection = glm::perspective(m_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void Hellospheres::draw() {
    OpenGLDemo::draw();

    m_shader->use();

    m_view = m_camera->viewmatrix();

    m_shader->setMat4("view", m_view);
    m_shader->setMat4("projection", m_projection);

    if (m_resetMeshes) {
        m_UVsphere->reset();
        m_Icosphere->reset();
        m_resetMeshes = false;
    }

    m_UVsphere->draw(*m_shader);
    m_Icosphere->draw(*m_shader);
}


void Hellospheres::mouseclick(int button, float xpos, float ypos) {
    m_button = button;
    m_mousex = xpos;
    m_mousey = ypos;
    m_camera->processmouseclick(m_button, xpos, ypos);
}

void Hellospheres::mousemove(float xpos, float ypos) {
    m_camera->processmousemovement(m_button, xpos, ypos, true);
}

void Hellospheres::keyboardmove(int key, double time) {
    m_camera->processkeyboard(Camera_Movement(key), time);
}

bool Hellospheres::keyboard(unsigned char k) {
    switch(k) {
        case 'p':
            m_activecamera = (m_activecamera+1)%2;
            m_camera.reset(m_cameraselector[m_activecamera]());
            m_camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
            return true;
        case 's':
            m_activeshader = (m_activeshader+1)%(unsigned)m_shaderselector.size();
            m_shader.reset(m_shaderselector[m_activeshader]());
            return true;
        case '+':
            m_Icosphere->subdivide();
            m_resetMeshes = true;
            return true;
        case '-':
            m_Icosphere->unsubdivide();
            m_resetMeshes = true;
            return true;
        case '*':
            m_UVsphere->setParallels(m_UVsphere->getParallels() + 1);
            m_UVsphere->setMeridians(m_UVsphere->getMeridians() + 1);
            m_resetMeshes = true;
            return true;
        case '/':
            m_UVsphere->setParallels(m_UVsphere->getParallels() - 1);
            m_UVsphere->setMeridians(m_UVsphere->getMeridians() - 1);
            m_resetMeshes = true;
            return true;
        default:
            return false;
    }
}

void Hellospheres::shaderChanged(ShaderSelection selected) {
    m_activeshader = selected;
    if (m_activeshader >= m_shaderselector.size())
        m_activeshader = m_shaderselector.size() - 1;
    m_shader.reset(m_shaderselector[m_activeshader]());
}
