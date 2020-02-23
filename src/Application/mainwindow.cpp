#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <Engine/Geometry/Model.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);

    ui->m_openglwidget->setFocus();
    ui->m_openglwidget->setFormat(format);

    connect(ui->x_position, SIGNAL(valueChanged(double)), this, SLOT(on_position_valueChanged(double)));
    connect(ui->y_position, SIGNAL(valueChanged(double)), this, SLOT(on_position_valueChanged(double)));
    connect(ui->z_position, SIGNAL(valueChanged(double)), this, SLOT(on_position_valueChanged(double)));
    connect(ui->x_rotation, SIGNAL(valueChanged(double)), this, SLOT(on_rotation_valueChanged(double)));
    connect(ui->y_rotation, SIGNAL(valueChanged(double)), this, SLOT(on_rotation_valueChanged(double)));
    connect(ui->z_rotation, SIGNAL(valueChanged(double)), this, SLOT(on_rotation_valueChanged(double)));
    connect(ui->x_scale, SIGNAL(valueChanged(double)), this, SLOT(on_scale_valueChanged(double)));
    connect(ui->y_scale, SIGNAL(valueChanged(double)), this, SLOT(on_scale_valueChanged(double)));
    connect(ui->z_scale, SIGNAL(valueChanged(double)), this, SLOT(on_scale_valueChanged(double)));

    connect(ui->m_openglwidget, &MyOpenGLWidget::selectionChanged, this, &MainWindow::on_selectionChanged);

    unset_settings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenGL_Version_triggered()
{
    std::stringstream message;
    message << "Qt version     : " << qVersion() << std::endl;
    message << "Renderer       : " << glGetString(GL_RENDERER) << std::endl;
    message << "Vendor         : " << glGetString(GL_VENDOR) << std::endl;
    message << "Version        : " << glGetString(GL_VERSION) << std::endl;
    message << "GLSL Version   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    QMessageBox::information(this, "OpenGL Information", message.str().c_str());
}

void MainWindow::on_position_valueChanged(double d) {
    if (m_state == DaftState::SELECTION)    return;
    glm::vec3 new_position {
        ui->x_position->value(),
        ui->y_position->value(),
        ui->z_position->value()
    };
    ui->m_openglwidget->getSelectedObject()->setPosition(new_position);
    ui->m_openglwidget->update();
}

void MainWindow::on_rotation_valueChanged(double d) {
    if (m_state == DaftState::SELECTION)    return;
    glm::vec3 new_rotation {
        ui->x_rotation->value(),
        ui->y_rotation->value(),
        ui->z_rotation->value()
    };
    ui->m_openglwidget->getSelectedObject()->setRotation(new_rotation);
    ui->m_openglwidget->update();
}

void MainWindow::on_scale_valueChanged(double d) {
    if (m_state == DaftState::SELECTION)    return;
    glm::vec3 new_scale {
        ui->x_scale->value(),
        ui->y_scale->value(),
        ui->z_scale->value()
    };
    ui->m_openglwidget->getSelectedObject()->setScale(new_scale);
    ui->m_openglwidget->update();
}

void MainWindow::on_selectionChanged(GLuint index) {
    m_state = DaftState::SELECTION;
    if (ui->m_openglwidget->getSelectedObject() == nullptr) return;
    ui->x_position->setValue(ui->m_openglwidget->getSelectedObject()->getPosition().x);
    ui->y_position->setValue(ui->m_openglwidget->getSelectedObject()->getPosition().y);
    ui->z_position->setValue(ui->m_openglwidget->getSelectedObject()->getPosition().z);
    ui->x_rotation->setValue(ui->m_openglwidget->getSelectedObject()->getRotation().x);
    ui->y_rotation->setValue(ui->m_openglwidget->getSelectedObject()->getRotation().y);
    ui->z_rotation->setValue(ui->m_openglwidget->getSelectedObject()->getRotation().z);
    ui->x_scale->setValue(ui->m_openglwidget->getSelectedObject()->getScale().x);
    ui->y_scale->setValue(ui->m_openglwidget->getSelectedObject()->getScale().y);
    ui->z_scale->setValue(ui->m_openglwidget->getSelectedObject()->getScale().z);

    ModelParam settings = ui->m_openglwidget->getSelectedObject()->getParams();
    ModelType type = ui->m_openglwidget->getSelectedObject()->getType();
    updateSettings(type);
    switch (type) {
    case ModelType::UV_SPHERE:
        ui->object_settings_label->setText("UVSphere Settings");
        ui->uv_meridians->setValue(settings.uv_sphere.meridians);
        ui->uv_parallels->setValue(settings.uv_sphere.parallels);
        break;
    case ModelType::ICO_SPHERE:
        ui->object_settings_label->setText("IcoSphere Settings");
        ui->ico_subdiv->setValue(settings.ico_sphere.subdivisions);
        break;
    case ModelType::CUBE_SPHERE:
        ui->object_settings_label->setText("CubeSphere Settings");
        ui->cube_res->setValue(settings.cube_sphere.resolution);
        break;
    case ModelType::MODEL:
        ui->object_settings_label->setText("Model Settings");
        break;
    }
    m_state = DaftState::EDIT;
}

void MainWindow::on_objectCreator_activated(const QString &arg1)
{
    if (arg1 == "UVSphere")
        ui->m_openglwidget->addObject(UV_SPHERE);
    else if (arg1 == "IcoSphere")
        ui->m_openglwidget->addObject(ICO_SPHERE);
    else if (arg1 == "CubeSphere")
        ui->m_openglwidget->addObject(CUBE_SPHERE);
    ui->m_openglwidget->setFocus();
    ui->objectCreator->setCurrentIndex(0);
}

void MainWindow::on_shaderChoser_activated(int index)
{
    ui->m_openglwidget->setShader((unsigned int)index);
    ui->m_openglwidget->setFocus();
}

void MainWindow::on_uv_meridians_valueChanged(int arg1)
{
    if (m_state == DaftState::SELECTION) return;
    ModelParam settings {};
    settings.uv_sphere.meridians = ui->uv_meridians->value();
    settings.uv_sphere.parallels = ui->uv_parallels->value();
    ui->m_openglwidget->setModelParams(UV_SPHERE, settings);
    ui->m_openglwidget->update();
}

void MainWindow::on_uv_parallels_valueChanged(int arg1)
{
    on_uv_meridians_valueChanged(arg1);
}

void MainWindow::on_ico_subdiv_valueChanged(int arg1)
{
    if (m_state == DaftState::SELECTION) return;
    ModelParam settings {};
    settings.ico_sphere.subdivisions = ui->ico_subdiv->value();
    ui->m_openglwidget->setModelParams(ICO_SPHERE, settings);
    ui->m_openglwidget->update();
}

void MainWindow::on_cube_res_valueChanged(int arg1)
{
    if (m_state == DaftState::SELECTION) return;
    ModelParam settings {};
    settings.cube_sphere.resolution = ui->cube_res->value();
    ui->m_openglwidget->setModelParams(CUBE_SPHERE, settings);
    ui->m_openglwidget->update();
}

void MainWindow::updateSettings(ModelType type) {
    unset_settings();
    switch (type) {
    case ModelType::UV_SPHERE:
        ui->uv_meridians->setVisible(true);
        ui->uv_parallels->setVisible(true);
        ui->uv_meridians_label->setVisible(true);
        ui->uv_parallels_label->setVisible(true);
        break;
    case ModelType::ICO_SPHERE:
        ui->ico_subdiv->setVisible(true);
        ui->ico_subdiv_label->setVisible(true);
        break;
    case ModelType::CUBE_SPHERE:
        ui->cube_res->setVisible(true);
        ui->cube_res_label->setVisible(true);
        break;
    case ModelType::MODEL:
        break;
    }
}

void MainWindow::unset_settings() {
    ui->uv_meridians->setVisible(false);
    ui->uv_parallels->setVisible(false);
    ui->uv_meridians_label->setVisible(false);
    ui->uv_parallels_label->setVisible(false);
    ui->ico_subdiv->setVisible(false);
    ui->ico_subdiv_label->setVisible(false);
    ui->cube_res->setVisible(false);
    ui->cube_res_label->setVisible(false);
}
