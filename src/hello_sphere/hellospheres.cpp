//
// Created by daftmat on 27/01/20.
//

#include "hellospheres.hpp"
#include <hello_sphere/Mesh.hpp>

Hellospheres::Hellospheres(int width, int height, Shader shader)
        : OpenGLDemo(width, height),
        m_shader { shader },
        m_activecamera { 0 },
        m_camera { nullptr } {
    m_cameraselector.push_back( []()->Camera*{ return new EulerCamera(glm::vec3(0.f, 0.f, 1.f)); } );
    m_cameraselector.push_back( []()->Camera*{ return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f)); } );

    m_camera.reset(m_cameraselector[m_activecamera]());
    m_camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));

    m_projection = glm::perspective(m_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
}

void Hellospheres::resize(int width, int height) {
    OpenGLDemo::resize(width, height);

    m_camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    m_projection = glm::perspective(m_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void Hellospheres::draw() {
    OpenGLDemo::draw();

    m_shader.use();

    m_shader.setMat4("view", m_camera->viewmatrix());
    m_shader.setMat4("projection", m_projection);

    for (auto &mesh : m_meshes) {
        mesh.draw(m_shader);
    }
}


void Hellospheres::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    m_camera->processmouseclick(_button, xpos, ypos);
}

void Hellospheres::mousemove(float xpos, float ypos) {
    m_camera->processmousemovement(_button, xpos, ypos, true);
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
        default:
            return false;
    }
}