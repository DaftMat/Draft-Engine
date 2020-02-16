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
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);

    ui->setupUi(this);

    m_openglwidget = new MyOpenGLWidget(ui->openglcontainer);
    m_openglwidget->resize(m_openglwidget->sizeHint());
    m_openglwidget->setFocus();
    m_openglwidget->setFormat(format);
}

MainWindow::~MainWindow()
{
    delete m_openglwidget;
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

void MainWindow::on_icoSphereButton_clicked()
{
    m_openglwidget->addObject(ICO_SPHERE);
    m_openglwidget->setFocus();
}

void MainWindow::on_cubeSphereButton_clicked()
{
    m_openglwidget->addObject(CUBE_SPHERE);
    m_openglwidget->setFocus();
}

void MainWindow::on_uvSphereButton_clicked()
{
    m_openglwidget->addObject(UV_SPHERE);
    m_openglwidget->setFocus();
}
