#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <Application/myopenglwidget.h>

namespace Ui {
class MainWindow;
}

enum DaftState {
    EDIT,
    SELECTION
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_actionOpenGL_Version_triggered();

    void on_position_valueChanged(double d);

    void on_rotation_valueChanged(double d);

    void on_scale_valueChanged(double d);

    void on_selectionChanged(GLuint index);

    void on_objectCreator_activated(const QString &arg1);

    void on_shaderChoser_activated(int index);

    void on_uv_meridians_valueChanged(int arg1);

    void on_uv_parallels_valueChanged(int arg1);

    void on_ico_subdiv_valueChanged(int arg1);

    void on_cube_res_valueChanged(int arg1);

    void on_deleteButton_clicked();

    void on_gizmoType_activated(const QString &arg1);

    void on_cubec_res_valueChanged(int arg1);

private:
    void updateSettings(ModelType type);
    void unset_settings();

    DaftState m_state { EDIT };
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
