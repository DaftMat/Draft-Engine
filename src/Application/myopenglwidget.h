#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QKeyEvent>

#include <memory>
#include <src/Engine/Engine.hpp>

/** QOpenGLWidget implementation.
 * Interface between Qt's MainWindow and my OpenGL Engine.
 */
class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
    Q_OBJECT

public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);

    ~MyOpenGLWidget() override;

    // size hints for the widget
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    /** Adds object with default settings to the scene.
     *  Function called by a MainWindow's event handler.
     * @param type - type of the created object.
     */
    void addObject(ModelType type);

    /** Adds light with default settings to the scene.
     *  Function called by a MainWindow's event handler.
     * @param type - type of the created light.
     */
    void addLight(LightType type);

    /** Deletes the selected entity (can either be a light or an object)
     *  Function called by a MainWindow's event handler.
     */
    void deleteObject();

    /** Selected object getter.
     *  Function used in MainWindow to get and set settings of the objects.
     * @return the selected object if there is one, nullptr otherwise.
     */
    Model * getSelectedObject() { return m_engine->getSelectedModel(); }

    /** Selected light getter.
     *  Function used in MainWindow to get and set settings of the lights.
     * @return the selected light if there is one, nullptr otherwise.
     */
    Light * getSelectedLight() { return m_engine->getSelectedLight(); }

    /** Object settings setter.
     *  Helper function used in MainWindow to set the settings of the objects.
     * @param params - Settings of the object
     */
    void setModelParams(const ModelParam &params) { m_engine->setModelParams(params); }

    /** Shader setter.
     *  Set the wanted shader used to render the scene.
     * @param index - index of the new shader
     */
    void setShader(GLuint index);

    /** EditionMode toggler.
     *  Toggles Edition mode - no edition mode means no grid, no gizmo, no mesh grid and a black sky.
     */
    void toggleEditionMode() { m_engine->toggleEditionMode(); }

    // Event management
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    /** Gizmo type setter
     *  Set the wanted gizmo type used to transform objects
     * @param type - type of the gizmo
     */
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
