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
    enum DaftState { EDIT, SELECTION };

    explicit MainWindow( QWidget* parent = nullptr );
    ~MainWindow() override;

  private slots:
    void on_actionOpenGL_Version_triggered();
    void on_position_valueChanged( double d );
    void on_rotation_valueChanged( double d );
    void on_scale_valueChanged( double d );
    void on_selectionChanged( GLuint index );
    void on_objectCreator_activated( const QString& arg1 );
    void on_shaderChoser_activated( int index );
    void on_uv_meridians_valueChanged( int arg1 );
    void on_uv_parallels_valueChanged( int arg1 );
    void on_ico_subdiv_valueChanged( int arg1 );
    void on_cube_res_valueChanged( int arg1 );
    void on_deleteButton_clicked();
    void on_gizmoType_activated( const QString& arg1 );
    void on_cubec_res_valueChanged( int arg1 );
    void on_editionToggle_clicked();
    void on_lightCreator_activated( const QString& arg1 );
    void on_colorSpin_valueChanged();
    void on_innercutSpin_valueChanged( double arg1 );
    void on_outercutSpin_valueChanged( double arg1 );
    void on_intensitySpin_valueChanged( double arg1 );
    void on_albedoSpin_valueChanged();
    void on_specularSpin_valueChanged();
    void on_roughnessSpin_valueChanged( double arg1 );
    void on_aoSpin_valueChanged( double arg1 );
    void on_iorSpin_valueChanged( double arg1 );
    void on_trianglesChanged( GLuint size );
    void on_renderButton_clicked();
    void on_transSpin_valueChanged(double arg1);
    void on_normalX_valueChanged(double arg1);
    void on_normalY_valueChanged(double arg1);
    void on_normalZ_valueChanged(double arg1);
    void on_varianceX_valueChanged(double arg1);
    void on_varianceY_valueChanged(double arg1);
    void on_varianceZ_valueChanged(double arg1);
    void on_freq_valueChanged(double arg1);
    void on_lifeTime_valueChanged(double arg1);
    void on_gravity_valueChanged(double arg1);
    void on_size_valueChanged(double arg1);
    void on_velocity_valueChanged(double arg1);

private:
    void updateSettings( Model::ModelType type );
    void updateSettings( Light::LightType type );
    void unset_settings();
    void objectSelection();
    void lightSelection();

    DaftState m_state{EDIT};
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
