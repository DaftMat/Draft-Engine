#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <Application/myopenglwidget.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenGL_Version_triggered();

    void on_icoSphereButton_clicked();

    void on_cubeSphereButton_clicked();

    void on_uvSphereButton_clicked();

private:
    Ui::MainWindow *ui;
    MyOpenGLWidget *m_openglwidget;
};

#endif // MAINWINDOW_H
