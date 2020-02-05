//
// Created by daftmat on 27/01/20.
//

#ifndef IIG3D_BASETP_HELLOSPHERES_HPP
#define IIG3D_BASETP_HELLOSPHERES_HPP

#include <memory>
#include <functional>

#include <opengldemo.h>

#include <hello_camera/camera.h>
#include <hello_sphere/Mesh.hpp>
#include <hello_sphere/Shader.hpp>

class Hellospheres : public OpenGLDemo {
public:
    explicit Hellospheres(int width, int height);
    ~Hellospheres() override;

    void resize(int width, int height) override;
    void draw() override;

    void mouseclick(int button, float xpos, float ypos) override;
    void mousemove(float xpos, float ypos) override;
    void keyboardmove(int key, double time) override;
    bool keyboard(unsigned char k) override;

private:
    // geometries
    std::vector<Mesh> m_meshes;

    // Shader
    using ShaderSelector=std::function<Shader*()>;
    std::vector<ShaderSelector> m_shaderselector;
    unsigned int m_activeshader;
    std::unique_ptr<Shader> m_shader;

    // mouse management
    int m_button; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float m_mousex{0};
    float m_mousey{0};

    // Camera
    using CameraSelector=std::function<Camera*()>;
    std::vector<CameraSelector> m_cameraselector;
    unsigned int m_activecamera;
    std::unique_ptr<Camera> m_camera;

    glm::mat4 m_projection;
};


#endif //IIG3D_BASETP_HELLOSPHERES_HPP
