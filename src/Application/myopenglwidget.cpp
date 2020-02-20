#include "myopenglwidget.h"

#include <QMessageBox>
#include <QApplication>
#include <QDateTime>

#include <iostream>
#include <stdexcept>

#include <src/Engine/Engine.hpp>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) :QOpenGLWidget(parent)/*, QOpenGLFunctions_4_1_Core()*/,
                                m_engine { nullptr }, _lastime { 0 }, frame_loop { false } {}

MyOpenGLWidget::~MyOpenGLWidget() {
}

QSize MyOpenGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyOpenGLWidget::sizeHint() const
{
    return QSize(720, 512);
}

void MyOpenGLWidget::cleanup() {
    m_engine.reset(nullptr);
}

void MyOpenGLWidget::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MyOpenGLWidget::cleanup);

    if (!initializeOpenGLFunctions()) {
        QMessageBox::critical(this, "OpenGL initialization error", "MyOpenGLWidget::initializeGL() : Unable to initialize OpenGL functions");
        exit(1);
    }
    // Initialize OpenGL and all OpenGL dependent stuff below
    m_engine.reset(new Engine(width(), height()));
}

void MyOpenGLWidget::paintGL() {
    std::int64_t starttime = QDateTime::currentMSecsSinceEpoch();
    m_engine->draw();
    glCheckError()
    glFinish();
    std::int64_t endtime = QDateTime::currentMSecsSinceEpoch();
    _lastime = endtime-starttime;
}

void MyOpenGLWidget::resizeGL(int width, int height) {
    m_engine->resize(width, height);
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    // buttons are 0(left), 1(right) to 2(middle)
    int b;
    Qt::MouseButton button=event->button();
    if (button & Qt::LeftButton) {
        if ((event->modifiers() & Qt::ControlModifier))
            b = 2;
        else
            b = 0;
    } else if (button & Qt::RightButton)
        b = 1;
    else if (button & Qt::MiddleButton)
        b = 2;
    else
        b = 3;
    m_engine->mouseclick(b, event->x(), event->y());
    _lastime = QDateTime::currentMSecsSinceEpoch();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    m_engine->mousemove(event->x(), event->y());
    update();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        // Move keys
        case Qt::Key_Left:
        case Qt::Key_Up:
        case Qt::Key_Right:
        case Qt::Key_Down:
            m_engine->keyboardmove(event->key()-Qt::Key_Left, 1./100/*double(_lastime)/10.*/);
            update();
        break;
        // Wireframe key
        case Qt::Key_W:
            m_engine->toggledrawmode();
            update();
        break;
        // Other keys are transmitted to the scene
        case Qt::Key_O:
            m_engine->keyboard('o');
            emit selectionChanged(m_engine->getSelectedIndex());
            update();
        break;
        default :
            if (m_engine->keyboard((unsigned char)event->text().toStdString()[0]))
                update();
        break;
    }
}

void MyOpenGLWidget::addObject(ModelType type) {
    m_engine->addModel(type);
    update();
}
