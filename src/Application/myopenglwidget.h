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

    Model & getSelectedObject() { return m_engine->getSelectedModel(); }

    // Event management
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

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
    bool frame_loop {false};
};

#endif // MYOPENGLWIDGET_H
