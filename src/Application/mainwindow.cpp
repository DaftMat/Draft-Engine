#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>

#include <iostream>
#include <sstream>
#include <iomanip>

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
    ui->m_openglwidget->getSelectedObject().setPosition(new_position);
    ui->m_openglwidget->update();
}

void MainWindow::on_rotation_valueChanged(double d) {
    if (m_state == DaftState::SELECTION)    return;
    glm::vec3 new_rotation {
        ui->x_rotation->value(),
        ui->y_rotation->value(),
        ui->z_rotation->value()
    };
    ui->m_openglwidget->getSelectedObject().setRotation(new_rotation);
    ui->m_openglwidget->update();
}

void MainWindow::on_scale_valueChanged(double d) {
    if (m_state == DaftState::SELECTION)    return;
    glm::vec3 new_scale {
        ui->x_scale->value(),
        ui->y_scale->value(),
        ui->z_scale->value()
    };
    ui->m_openglwidget->getSelectedObject().setScale(new_scale);
    ui->m_openglwidget->update();
}

void MainWindow::on_selectionChanged(GLuint index) {
    m_state = DaftState::SELECTION;
    ui->x_position->setValue(ui->m_openglwidget->getSelectedObject().getPosition().x);
    ui->y_position->setValue(ui->m_openglwidget->getSelectedObject().getPosition().y);
    ui->z_position->setValue(ui->m_openglwidget->getSelectedObject().getPosition().z);
    ui->x_rotation->setValue(ui->m_openglwidget->getSelectedObject().getRotation().x);
    ui->y_rotation->setValue(ui->m_openglwidget->getSelectedObject().getRotation().y);
    ui->z_rotation->setValue(ui->m_openglwidget->getSelectedObject().getRotation().z);
    ui->x_scale->setValue(ui->m_openglwidget->getSelectedObject().getScale().x);
    ui->y_scale->setValue(ui->m_openglwidget->getSelectedObject().getScale().y);
    ui->z_scale->setValue(ui->m_openglwidget->getSelectedObject().getScale().z);
    m_state = DaftState::EDIT;
}

void MainWindow::on_objectCreator_activated(const QString &arg1)
{
    if (arg1 == "Create Object")    return;
    else if (arg1 == "UVSphere")
        ui->m_openglwidget->addObject(UV_SPHERE);
    else if (arg1 == "IcoSphere")
        ui->m_openglwidget->addObject(ICO_SPHERE);
    else if (arg1 == "CubeSphere")
        ui->m_openglwidget->addObject(CUBE_SPHERE);
    ui->m_openglwidget->setFocus();
    ui->objectCreator->setCurrentIndex(0);
}
