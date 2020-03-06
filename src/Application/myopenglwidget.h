#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QKeyEvent>

#include <memory>
#include <src/Engine/Engine.hpp>


class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
    Q_OBJECT

public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);

    ~MyOpenGLWidget() override;

    // size hints for the widget
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void addObject(ModelType type);
    void addLight(LightType type);
    void deleteObject();

    Model * getSelectedObject() { return m_engine->getSelectedModel(); }
    Light * getSelectedLight() { return m_engine->getSelectedLight(); }

    void setModelParams(const ModelParam &params) { m_engine->setModelParams(params); }

    void setShader(GLuint index);
    void toggleEditionMode() { m_engine->toggleEditionMode(); }

    // Event management
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void setGizmoType(GizmoType type) { m_engine->setGizmoType(type); update(); }

public slots:
    void cleanup();

signals:
    void selectionChanged(GLuint newIndex);

protected:
    // OpenGL management
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private :
    std::unique_ptr<Engine> m_engine;

    // for event management
    std::int64_t _lastime;
    bool object_added;
};

#endif // MYOPENGLWIDGET_H
