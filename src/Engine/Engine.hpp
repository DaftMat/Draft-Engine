//
// Created by daftmat on 27/01/20.
//

#ifndef IIG3D_BASETP_HELLOSPHERES_HPP
#define IIG3D_BASETP_HELLOSPHERES_HPP

#include <memory>
#include <functional>

#include <src/Engine/Cameras/camera.h>
#include <Engine/Shader.hpp>
#include <src/Engine/Managers/ModelManager.hpp>

class Engine {
public:
    explicit Engine(int width, int height);
    ~Engine();

    void resize(int width, int height);
    void draw();

    void mouseclick(int button, float xpos, float ypos);
    void mouserelease();
    void mousemove(float xpos, float ypos);
    void keyboardmove(int key, double time);
    bool keyboard(unsigned char k);
    void shaderChanged(GLuint selected);

    void toggledrawmode() { m_modelmanager->toggledrawmode(); }

    void addModel(ModelType type);
    void deleteModel() { m_modelmanager->deleteModel(); }

    Model * getSelectedModel() { return m_modelmanager->getSelectedObject(); }
    GLuint getSelectedIndex() { return m_modelmanager->getSelectedIndex(); }
    void setSelectedIndex(GLuint index) { m_modelmanager->setSelectedIndex(index); }

    void setModelParams(const ModelParam &params);

    void setGizmoType(GizmoType type) { m_modelmanager->switchGizmo(type); }

private:
    void checkCreation();

    int m_width;
    int m_height;

    struct ModelCreation {
        bool toCreate;
        ModelType type;
    } m_creationstate;

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
    bool m_clicked_object{false};

    // Camera
    using CameraSelector=std::function<Camera*()>;
    std::vector<CameraSelector> m_cameraselector;
    unsigned int m_activecamera;
    std::unique_ptr<Camera> m_camera;

    glm::mat4 m_projection;

};


#endif //IIG3D_BASETP_HELLOSPHERES_HPP
