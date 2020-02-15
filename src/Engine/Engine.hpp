//
// Created by daftmat on 27/01/20.
//

#ifndef IIG3D_BASETP_HELLOSPHERES_HPP
#define IIG3D_BASETP_HELLOSPHERES_HPP

#include <memory>
#include <functional>

#include <src/Engine/Cameras/camera.h>
#include <src/Engine/Geometry/Mesh.hpp>
#include <Engine/Shader.hpp>
#include <src/Engine/Geometry/Primitives/UVSphere.hpp>
#include <src/Engine/Managers/ModelManager.hpp>
#include "src/Engine/Geometry/Primitives/IcoSphere.hpp"

class Engine {
public:
    explicit Engine(int width, int height);
    ~Engine();

    void resize(int width, int height);
    void draw();

    void mouseclick(int button, float xpos, float ypos);
    void mousemove(float xpos, float ypos);
    void keyboardmove(int key, double time);
    bool keyboard(unsigned char k);
    void shaderChanged(ShaderSelection selected);

    void toggledrawmode() {
        m_modelmanager->toggledrawmode();
    }

    void addUVSphere() { m_modelmanager->addUVSphere(); }
    void addIcoSphere() { m_modelmanager->addIcoSphere(); }
    void addCubeSphere() { m_modelmanager->addCubeSphere(); }

private:
    int m_width;
    int m_height;

    // geometries
    std::unique_ptr<ModelManager> m_modelmanager;

    // Shader
    using ShaderSelector=std::function<Shader*()>;
    std::vector<ShaderSelector> m_shaderselector;
    unsigned int m_activeshader;
    std::unique_ptr<Shader> m_shader;

    // mouse management
    int m_button{0}; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float m_mousex{0};
    float m_mousey{0};

    // Camera
    using CameraSelector=std::function<Camera*()>;
    std::vector<CameraSelector> m_cameraselector;
    unsigned int m_activecamera;
    std::unique_ptr<Camera> m_camera;

    glm::mat4 m_view;
    glm::mat4 m_projection;
};


#endif //IIG3D_BASETP_HELLOSPHERES_HPP
