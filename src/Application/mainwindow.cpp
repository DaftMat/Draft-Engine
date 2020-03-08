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

    connect(ui->ambientRSpin, SIGNAL(valueChanged(int)), this, SLOT(on_ambientSpin_valueChanged()));
    connect(ui->ambientGSpin, SIGNAL(valueChanged(int)), this, SLOT(on_ambientSpin_valueChanged()));
    connect(ui->ambientBSpin, SIGNAL(valueChanged(int)), this, SLOT(on_ambientSpin_valueChanged()));
    connect(ui->diffuseRSpin, SIGNAL(valueChanged(int)), this, SLOT(on_diffuseSpin_valueChanged()));
    connect(ui->diffuseGSpin, SIGNAL(valueChanged(int)), this, SLOT(on_diffuseSpin_valueChanged()));
    connect(ui->diffuseBSpin, SIGNAL(valueChanged(int)), this, SLOT(on_diffuseSpin_valueChanged()));
    connect(ui->specularRSpin, SIGNAL(valueChanged(int)), this, SLOT(on_specularSpin_valueChanged()));
    connect(ui->specularGSpin, SIGNAL(valueChanged(int)), this, SLOT(on_specularSpin_valueChanged()));
    connect(ui->specularBSpin, SIGNAL(valueChanged(int)), this, SLOT(on_specularSpin_valueChanged()));

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
    if (ui->m_openglwidget->getSelectedObject() != nullptr)
        ui->m_openglwidget->getSelectedObject()->setPosition(new_position);
    if (ui->m_openglwidget->getSelectedLight() != nullptr) {
        LightParam parampos = ui->m_openglwidget->getSelectedLight()->getParams();
        switch(ui->m_openglwidget->getSelectedLight()->getType()) {
        case POINT_LIGHT:
            parampos.pointlight.position = new_position;
            break;
        case SPOT_LIGHT:
            parampos.spotlight.position = new_position;
            break;
        default:
            break;
        }
        ui->m_openglwidget->getSelectedLight()->editLight(parampos);
    }
    ui->m_openglwidget->update();
}

void MainWindow::on_rotation_valueChanged(double d) {
    if (m_state == DaftState::SELECTION)    return;
    glm::vec3 new_rotation {
        ui->x_rotation->value(),
        ui->y_rotation->value(),
        ui->z_rotation->value()
    };
    if (ui->m_openglwidget->getSelectedObject() != nullptr)
        ui->m_openglwidget->getSelectedObject()->setRotation(new_rotation);
    if (ui->m_openglwidget->getSelectedLight() != nullptr) {
        LightParam parampos = ui->m_openglwidget->getSelectedLight()->getParams();
        switch(ui->m_openglwidget->getSelectedLight()->getType()) {
            case DIR_LIGHT:
                parampos.dirlight.rotations = new_rotation;
                break;
            case SPOT_LIGHT:
                parampos.spotlight.rotations = new_rotation;
                break;
            default:
                break;
        }
        ui->m_openglwidget->getSelectedLight()->editLight(parampos);
    }
    ui->m_openglwidget->update();
}

void MainWindow::on_scale_valueChanged(double d) {
    if (m_state == DaftState::SELECTION)    return;
    glm::vec3 new_scale {
        ui->x_scale->value(),
        ui->y_scale->value(),
        ui->z_scale->value()
    };
    if (ui->m_openglwidget->getSelectedObject() != nullptr)
        ui->m_openglwidget->getSelectedObject()->setScale(new_scale);
    ui->m_openglwidget->update();
}

void MainWindow::on_selectionChanged(GLuint index) {
    m_state = DaftState::SELECTION;
    unset_settings();
    objectSelection();
    lightSelection();
    m_state = DaftState::EDIT;
}

void MainWindow::objectSelection() {
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
        case ModelType::CUBE:
            ui->object_settings_label->setText("Cube Settings");
            ui->cubec_res->setValue(settings.cube.resolution);
            break;
        default:
            ui->object_settings_label->setText("Model Settings");
            break;
    }
}

void MainWindow::lightSelection() {
    if (ui->m_openglwidget->getSelectedLight() == nullptr)  return;
    ui->x_position->setValue(ui->m_openglwidget->getSelectedLight()->model().getPosition().x);
    ui->y_position->setValue(ui->m_openglwidget->getSelectedLight()->model().getPosition().y);
    ui->z_position->setValue(ui->m_openglwidget->getSelectedLight()->model().getPosition().z);
    ui->x_rotation->setValue(ui->m_openglwidget->getSelectedLight()->model().getRotation().x);
    ui->y_rotation->setValue(ui->m_openglwidget->getSelectedLight()->model().getRotation().y);
    ui->z_rotation->setValue(ui->m_openglwidget->getSelectedLight()->model().getRotation().z);
    ui->x_scale->setValue(1.0);
    ui->y_scale->setValue(1.0);
    ui->z_scale->setValue(1.0);

    LightParam settings = ui->m_openglwidget->getSelectedLight()->getParams();
    glm::vec3 ambient = ui->m_openglwidget->getSelectedLight()->ambient();
    glm::vec3 diffuse = ui->m_openglwidget->getSelectedLight()->diffuse();
    glm::vec3 specular = ui->m_openglwidget->getSelectedLight()->specular();
    LightType type = ui->m_openglwidget->getSelectedLight()->getType();
    updateSettings(type);
    switch(type) {
    case LightType::POINT_LIGHT:
        ui->object_settings_label->setText("Point Light Settings");
        ui->constantSpin->setValue(settings.pointlight.constant);
        ui->linearSpin->setValue(settings.pointlight.linear);
        ui->quadraticSpin->setValue(settings.pointlight.quadratic);
        break;
    case LightType::SPOT_LIGHT:
        ui->object_settings_label->setText("Spot Light Settings");
        ui->constantSpin->setValue(settings.spotlight.constant);
        ui->linearSpin->setValue(settings.spotlight.linear);
        ui->quadraticSpin->setValue(settings.spotlight.quadratic);
        ui->innercutSpin->setValue(settings.spotlight.innerCutoff);
        ui->outercutSpin->setValue(settings.spotlight.outerCutoff);
    default:
        break;
    }
    ui->ambientRSpin->setValue(int(ambient.r * 255.f));
    ui->ambientGSpin->setValue(int(ambient.g * 255.f));
    ui->ambientBSpin->setValue(int(ambient.b * 255.f));
    ui->diffuseRSpin->setValue(int(diffuse.r * 255.f));
    ui->diffuseGSpin->setValue(int(diffuse.g * 255.f));
    ui->diffuseBSpin->setValue(int(diffuse.b * 255.f));
    ui->specularRSpin->setValue(int(specular.r * 255.f));
    ui->specularGSpin->setValue(int(specular.g * 255.f));
    ui->specularBSpin->setValue(int(specular.b * 255.f));
}

void MainWindow::on_objectCreator_activated(const QString &arg1)
{
    if (arg1 == "UVSphere")
        ui->m_openglwidget->addObject(UV_SPHERE);
    else if (arg1 == "IcoSphere")
        ui->m_openglwidget->addObject(ICO_SPHERE);
    else if (arg1 == "CubeSphere")
        ui->m_openglwidget->addObject(CUBE_SPHERE);
    else if (arg1 == "Cube")
        ui->m_openglwidget->addObject(CUBE);
    ui->m_openglwidget->setFocus();
    ui->objectCreator->setCurrentIndex(0);
}

void MainWindow::on_lightCreator_activated(const QString &arg1)
{
    if (arg1 == "Point Light")
        ui->m_openglwidget->addLight(POINT_LIGHT);
    else if (arg1 == "Spot Light")
        ui->m_openglwidget->addLight(SPOT_LIGHT);
    else if (arg1 == "Directionnal Light")
        ui->m_openglwidget->addLight(DIR_LIGHT);
    ui->m_openglwidget->setFocus();
    ui->lightCreator->setCurrentIndex(0);
}


void MainWindow::on_shaderChoser_activated(int index)
{
    ui->m_openglwidget->setShader(GLuint(index));
    ui->m_openglwidget->setFocus();
}

void MainWindow::on_uv_meridians_valueChanged(int arg1)
{
    if (m_state == DaftState::SELECTION) return;
    ModelParam settings {};
    settings.uv_sphere.meridians = ui->uv_meridians->value();
    settings.uv_sphere.parallels = ui->uv_parallels->value();
    ui->m_openglwidget->setModelParams(settings);
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
    ui->m_openglwidget->setModelParams(settings);
    ui->m_openglwidget->update();
}

void MainWindow::on_cube_res_valueChanged(int arg1)
{
    if (m_state == DaftState::SELECTION) return;
    ModelParam settings {};
    settings.cube_sphere.resolution = ui->cube_res->value();
    ui->m_openglwidget->setModelParams(settings);
    ui->m_openglwidget->update();
}

void MainWindow::on_cubec_res_valueChanged(int arg1)
{
    if (m_state == DaftState::SELECTION) return;
    ModelParam settings {};
    settings.cube.resolution = arg1;
    ui->m_openglwidget->setModelParams(settings);
    ui->m_openglwidget->update();
}

void MainWindow::updateSettings(ModelType type) {
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
    case ModelType::CUBE:
        ui->cubec_res->setVisible(true);
        ui->cubec_res_label->setVisible(true);
    default:
        break;
    }
}

void MainWindow::updateSettings(LightType type) {
    ui->colorLightLabel->setVisible(true);
    ui->diffuseLabel->setVisible(true);
    ui->diffuseRSpin->setVisible(true);
    ui->diffuseGSpin->setVisible(true);
    ui->diffuseBSpin->setVisible(true);
    ui->ambientLabel->setVisible(true);
    ui->ambientRSpin->setVisible(true);
    ui->ambientGSpin->setVisible(true);
    ui->ambientBSpin->setVisible(true);
    ui->specularLabel->setVisible(true);
    ui->specularRSpin->setVisible(true);
    ui->specularGSpin->setVisible(true);
    ui->specularBSpin->setVisible(true);
    switch (type) {
    case LightType::POINT_LIGHT:
        ui->constantSpin->setVisible(true);
        ui->constantLabel->setVisible(true);
        ui->linearSpin->setVisible(true);
        ui->linearLabel->setVisible(true);
        ui->quadraticSpin->setVisible(true);
        ui->quadraticLabel->setVisible(true);
        break;
    case LightType::SPOT_LIGHT:
        ui->constantSpin->setVisible(true);
        ui->constantLabel->setVisible(true);
        ui->linearSpin->setVisible(true);
        ui->linearLabel->setVisible(true);
        ui->quadraticSpin->setVisible(true);
        ui->quadraticLabel->setVisible(true);
        ui->innercutSpin->setVisible(true);
        ui->innercutLabel->setVisible(true);
        ui->outercutSpin->setVisible(true);
        ui->outercutLabel->setVisible(true);
    default:
        break;
    }
}

void MainWindow::unset_settings() {
    ui->x_position->setValue(0.0);
    ui->y_position->setValue(0.0);
    ui->z_position->setValue(0.0);
    ui->x_rotation->setValue(0.0);
    ui->y_rotation->setValue(0.0);
    ui->z_rotation->setValue(0.0);
    ui->x_scale->setValue(1.0);
    ui->y_scale->setValue(1.0);
    ui->z_scale->setValue(1.0);
    //object settings
    ui->object_settings_label->setText("Model Settings");
    ui->uv_meridians->setVisible(false);
    ui->uv_parallels->setVisible(false);
    ui->uv_meridians_label->setVisible(false);
    ui->uv_parallels_label->setVisible(false);
    ui->ico_subdiv->setVisible(false);
    ui->ico_subdiv_label->setVisible(false);
    ui->cube_res->setVisible(false);
    ui->cube_res_label->setVisible(false);
    ui->cubec_res->setVisible(false);
    ui->cubec_res_label->setVisible(false);
    //light settings
    ui->constantSpin->setVisible(false);
    ui->constantLabel->setVisible(false);
    ui->linearSpin->setVisible(false);
    ui->linearLabel->setVisible(false);
    ui->quadraticSpin->setVisible(false);
    ui->quadraticLabel->setVisible(false);
    ui->colorLightLabel->setVisible(false);
    ui->ambientLabel->setVisible(false);
    ui->ambientRSpin->setVisible(false);
    ui->ambientGSpin->setVisible(false);
    ui->ambientBSpin->setVisible(false);
    ui->diffuseLabel->setVisible(false);
    ui->diffuseRSpin->setVisible(false);
    ui->diffuseGSpin->setVisible(false);
    ui->diffuseBSpin->setVisible(false);
    ui->specularLabel->setVisible(false);
    ui->specularRSpin->setVisible(false);
    ui->specularGSpin->setVisible(false);
    ui->specularBSpin->setVisible(false);
    ui->innercutSpin->setVisible(false);
    ui->innercutLabel->setVisible(false);
    ui->outercutSpin->setVisible(false);
    ui->outercutLabel->setVisible(false);
}

void MainWindow::on_deleteButton_clicked()
{
    ui->m_openglwidget->deleteObject();
}

void MainWindow::on_gizmoType_activated(const QString &arg1)
{
    if (arg1 == "Translation") {
        ui->m_openglwidget->setGizmoType(TRANSLATE);
    } else if (arg1 == "Scale") {
        ui->m_openglwidget->setGizmoType(SCALE);
    }
}

void MainWindow::on_editionToggle_clicked()
{
    ui->m_openglwidget->toggleEditionMode();
    ui->m_openglwidget->setFocus();
    ui->m_openglwidget->update();
}

void MainWindow::on_ambientSpin_valueChanged() {
    if (m_state == SELECTION)   return;
    glm::vec3 new_ambient {
        ui->ambientRSpin->value() / 255.f,
        ui->ambientGSpin->value() / 255.f,
        ui->ambientBSpin->value() / 255.f
    };
    if (ui->m_openglwidget->getSelectedLight() != nullptr)
        ui->m_openglwidget->getSelectedLight()->ambient() = new_ambient;
    ui->m_openglwidget->update();
}

void MainWindow::on_diffuseSpin_valueChanged() {
    if (m_state == SELECTION)   return;
    glm::vec3 new_diffuse {
        ui->diffuseRSpin->value() / 255.f,
        ui->diffuseGSpin->value() / 255.f,
        ui->diffuseBSpin->value() / 255.f
    };
    if (ui->m_openglwidget->getSelectedLight() != nullptr)
        ui->m_openglwidget->getSelectedLight()->diffuse() = new_diffuse;
    ui->m_openglwidget->update();
}

void MainWindow::on_specularSpin_valueChanged() {
    if (m_state == SELECTION)   return;
    glm::vec3 new_specular {
        ui->specularRSpin->value() / 255.f,
        ui->specularGSpin->value() / 255.f,
        ui->specularBSpin->value() / 255.f
    };
    if (ui->m_openglwidget->getSelectedLight() != nullptr)
        ui->m_openglwidget->getSelectedLight()->specular() = new_specular;
    ui->m_openglwidget->update();
}

void MainWindow::on_constantSpin_valueChanged(double arg1)
{
    if (m_state == SELECTION)   return;
    if (ui->m_openglwidget->getSelectedLight() == nullptr)  return;
    LightParam params = ui->m_openglwidget->getSelectedLight()->getParams();
    switch(ui->m_openglwidget->getSelectedLight()->getType()) {
    case POINT_LIGHT:
        params.pointlight.constant = float(arg1);
        break;
    case SPOT_LIGHT:
        params.spotlight.constant = float(arg1);
        break;
    default:
        break;
    }
    ui->m_openglwidget->getSelectedLight()->editLight(params);
    ui->m_openglwidget->update();
}

void MainWindow::on_linearSpin_valueChanged(double arg1)
{
    if (m_state == SELECTION)   return;
    if (ui->m_openglwidget->getSelectedLight() == nullptr)  return;
    LightParam params = ui->m_openglwidget->getSelectedLight()->getParams();
    switch(ui->m_openglwidget->getSelectedLight()->getType()) {
    case POINT_LIGHT:
        params.pointlight.linear = float(arg1);
        break;
    case SPOT_LIGHT:
        params.spotlight.linear = float(arg1);
        break;
    default:
        break;
    }
    ui->m_openglwidget->getSelectedLight()->editLight(params);
    ui->m_openglwidget->update();
}

void MainWindow::on_quadraticSpin_valueChanged(double arg1)
{
    if (m_state == SELECTION)   return;
    if (ui->m_openglwidget->getSelectedLight() == nullptr)  return;
    LightParam params = ui->m_openglwidget->getSelectedLight()->getParams();
    switch(ui->m_openglwidget->getSelectedLight()->getType()) {
    case POINT_LIGHT:
        params.pointlight.quadratic = float(arg1);
        break;
    case SPOT_LIGHT:
        params.spotlight.quadratic = float(arg1);
        break;
    default:
        break;
    }
    ui->m_openglwidget->getSelectedLight()->editLight(params);
    ui->m_openglwidget->update();
}

void MainWindow::on_innercutSpin_valueChanged(double arg1)
{
    if (m_state == SELECTION) return;
    if (ui->m_openglwidget->getSelectedLight() == nullptr)  return;
    LightParam params = ui->m_openglwidget->getSelectedLight()->getParams();
    if (ui->m_openglwidget->getSelectedLight()->getType() == LightType::SPOT_LIGHT) {
        if (float(arg1) > params.spotlight.outerCutoff) {
            ui->innercutSpin->setValue(params.spotlight.innerCutoff);
            return;
        }
        params.spotlight.innerCutoff = float(arg1);
        ui->m_openglwidget->getSelectedLight()->editLight(params);
    }
    ui->m_openglwidget->update();
}

void MainWindow::on_outercutSpin_valueChanged(double arg1)
{
    if (m_state == SELECTION) return;
    if (ui->m_openglwidget->getSelectedLight() == nullptr)  return;
    LightParam params = ui->m_openglwidget->getSelectedLight()->getParams();
    if (ui->m_openglwidget->getSelectedLight()->getType() == LightType::SPOT_LIGHT) {
        if (float(arg1) < params.spotlight.innerCutoff) {
            ui->outercutSpin->setValue(params.spotlight.outerCutoff);
            return;
        }
        params.spotlight.outerCutoff = float(arg1);
        ui->m_openglwidget->getSelectedLight()->editLight(params);
    }
    ui->m_openglwidget->update();
}
