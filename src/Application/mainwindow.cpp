#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>

#include <iomanip>
#include <iostream>
#include <sstream>

#include <Engine/Geometry/Model.hpp>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    QSurfaceFormat format;
    format.setVersion( 4, 1 );
    format.setProfile( QSurfaceFormat::CoreProfile );
    format.setDepthBufferSize( 24 );
    format.setSamples( 16 );
    QSurfaceFormat::setDefaultFormat( format );

    ui->m_openglwidget->setFocus();
    ui->m_openglwidget->setFormat( format );

    connect( ui->x_position,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_position_valueChanged( double ) ) );
    connect( ui->y_position,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_position_valueChanged( double ) ) );
    connect( ui->z_position,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_position_valueChanged( double ) ) );
    connect( ui->x_rotation,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_rotation_valueChanged( double ) ) );
    connect( ui->y_rotation,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_rotation_valueChanged( double ) ) );
    connect( ui->z_rotation,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_rotation_valueChanged( double ) ) );
    connect( ui->x_scale,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_scale_valueChanged( double ) ) );
    connect( ui->y_scale,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_scale_valueChanged( double ) ) );
    connect( ui->z_scale,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( on_scale_valueChanged( double ) ) );

    connect(
        ui->colorRSpin, SIGNAL( valueChanged( int ) ), this, SLOT( on_colorSpin_valueChanged() ) );
    connect(
        ui->colorGSpin, SIGNAL( valueChanged( int ) ), this, SLOT( on_colorSpin_valueChanged() ) );
    connect(
        ui->colorBSpin, SIGNAL( valueChanged( int ) ), this, SLOT( on_colorSpin_valueChanged() ) );

    connect( ui->albedoRspin,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( on_albedoSpin_valueChanged() ) );
    connect( ui->albedoGspin,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( on_albedoSpin_valueChanged() ) );
    connect( ui->albedoBspin,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( on_albedoSpin_valueChanged() ) );
    connect( ui->specularRspin,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( on_specularSpin_valueChanged() ) );
    connect( ui->specularGspin,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( on_specularSpin_valueChanged() ) );
    connect( ui->specularBspin,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( on_specularSpin_valueChanged() ) );

    connect( ui->m_openglwidget,
             &MyOpenGLWidget::selectionChanged,
             this,
             &MainWindow::on_selectionChanged );

    connect( ui->m_openglwidget,
             &MyOpenGLWidget::trianglesChanged,
             this,
             &MainWindow::on_trianglesChanged );

    unset_settings();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionOpenGL_Version_triggered() {
    std::stringstream message;
    message << "Qt version     : " << qVersion() << std::endl;
    message << "Renderer       : " << glGetString( GL_RENDERER ) << std::endl;
    message << "Vendor         : " << glGetString( GL_VENDOR ) << std::endl;
    message << "Version        : " << glGetString( GL_VERSION ) << std::endl;
    message << "GLSL Version   : " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;
    QMessageBox::information( this, "OpenGL Information", message.str().c_str() );
}

void MainWindow::on_position_valueChanged( double d ) {
    if ( m_state == DaftState::SELECTION ) return;
    glm::vec3 new_position{
        ui->x_position->value(), ui->y_position->value(), ui->z_position->value()};
    if ( ui->m_openglwidget->getSelectedObject() != nullptr )
        ui->m_openglwidget->getSelectedObject()->setPosition( new_position );
    if ( ui->m_openglwidget->getSelectedLight() != nullptr )
    {
        Light::LightParam parampos = ui->m_openglwidget->getSelectedLight()->getParams();
        switch ( ui->m_openglwidget->getSelectedLight()->getType() )
        {
        case Light::POINT_LIGHT:
            parampos.pointlight.position = new_position;
            break;
        case Light::SPOT_LIGHT:
            parampos.spotlight.position = new_position;
            break;
        default:
            break;
        }
        ui->m_openglwidget->getSelectedLight()->editLight( parampos );
    }
    ui->m_openglwidget->update();
}

void MainWindow::on_rotation_valueChanged( double d ) {
    if ( m_state == DaftState::SELECTION ) return;
    glm::vec3 new_rotation{
        ui->x_rotation->value(), ui->y_rotation->value(), ui->z_rotation->value()};
    if ( ui->m_openglwidget->getSelectedObject() != nullptr )
        ui->m_openglwidget->getSelectedObject()->setRotation( new_rotation );
    if ( ui->m_openglwidget->getSelectedLight() != nullptr )
    {
        Light::LightParam parampos = ui->m_openglwidget->getSelectedLight()->getParams();
        switch ( ui->m_openglwidget->getSelectedLight()->getType() )
        {
        case Light::DIR_LIGHT:
            parampos.dirlight.rotations = new_rotation;
            break;
        case Light::SPOT_LIGHT:
            parampos.spotlight.rotations = new_rotation;
            break;
        default:
            break;
        }
        ui->m_openglwidget->getSelectedLight()->editLight( parampos );
    }
    ui->m_openglwidget->update();
}

void MainWindow::on_scale_valueChanged( double d ) {
    if ( m_state == DaftState::SELECTION ) return;
    glm::vec3 new_scale{ui->x_scale->value(), ui->y_scale->value(), ui->z_scale->value()};
    if ( ui->m_openglwidget->getSelectedObject() != nullptr )
        ui->m_openglwidget->getSelectedObject()->setScale( new_scale );
    ui->m_openglwidget->update();
}

void MainWindow::on_selectionChanged( GLuint index ) {
    m_state = DaftState::SELECTION;
    unset_settings();
    objectSelection();
    lightSelection();
    m_state = DaftState::EDIT;
}

void MainWindow::objectSelection() {
    if ( ui->m_openglwidget->getSelectedObject() == nullptr ) return;
    ui->x_position->setValue( ui->m_openglwidget->getSelectedObject()->getPosition().x );
    ui->y_position->setValue( ui->m_openglwidget->getSelectedObject()->getPosition().y );
    ui->z_position->setValue( ui->m_openglwidget->getSelectedObject()->getPosition().z );
    ui->x_rotation->setValue( ui->m_openglwidget->getSelectedObject()->getRotation().x );
    ui->y_rotation->setValue( ui->m_openglwidget->getSelectedObject()->getRotation().y );
    ui->z_rotation->setValue( ui->m_openglwidget->getSelectedObject()->getRotation().z );
    ui->x_scale->setValue( ui->m_openglwidget->getSelectedObject()->getScale().x );
    ui->y_scale->setValue( ui->m_openglwidget->getSelectedObject()->getScale().y );
    ui->z_scale->setValue( ui->m_openglwidget->getSelectedObject()->getScale().z );

    Model::ModelParam settings = ui->m_openglwidget->getSelectedObject()->getParams();
    Model::ModelType type      = ui->m_openglwidget->getSelectedObject()->getType();
    GLuint size                = ui->m_openglwidget->getSelectedObject()->getSize();
    updateSettings( type );
    switch ( type )
    {
    case Model::ModelType::UV_SPHERE:
        ui->object_settings_label->setText( "UVSphere Settings" );
        ui->uv_meridians->setValue( settings.uv_sphere.meridians );
        ui->uv_parallels->setValue( settings.uv_sphere.parallels );
        break;
    case Model::ModelType::ICO_SPHERE:
        ui->object_settings_label->setText( "IcoSphere Settings" );
        ui->ico_subdiv->setValue( settings.ico_sphere.subdivisions );
        break;
    case Model::ModelType::CUBE_SPHERE:
        ui->object_settings_label->setText( "CubeSphere Settings" );
        ui->cube_res->setValue( settings.cube_sphere.resolution );
        break;
    case Model::ModelType::CUBE:
        ui->object_settings_label->setText( "Cube Settings" );
        ui->cubec_res->setValue( settings.cube.resolution );
        break;
    default:
        ui->object_settings_label->setText( "Model Settings" );
        break;
    }
    ui->triangle_num->setText( ( "Number of triangles : " + std::to_string( size ) ).c_str() );

    ui->materialSettingsWidget->setEnabled( true );
    Material material = ui->m_openglwidget->getSelectedObject()->material();
    ui->albedoRspin->setValue( int( material.albedo().r * 255.f ) );
    ui->albedoGspin->setValue( int( material.albedo().g * 255.f ) );
    ui->albedoBspin->setValue( int( material.albedo().b * 255.f ) );
    ui->specularRspin->setValue( int( material.specular().r * 255.f ) );
    ui->specularGspin->setValue( int( material.specular().g * 255.f ) );
    ui->specularBspin->setValue( int( material.specular().b * 255.f ) );
    ui->roughnessSpin->setValue( material.roughness() );
    ui->aoSpin->setValue( material.ambientOcclusion() );
    ui->iorSpin->setValue( material.IOR() );
    ui->transSpin->setValue( material.transparency() );
}

void MainWindow::lightSelection() {
    if ( ui->m_openglwidget->getSelectedLight() == nullptr ) return;
    ui->x_position->setValue( ui->m_openglwidget->getSelectedLight()->model().getPosition().x );
    ui->y_position->setValue( ui->m_openglwidget->getSelectedLight()->model().getPosition().y );
    ui->z_position->setValue( ui->m_openglwidget->getSelectedLight()->model().getPosition().z );
    ui->x_rotation->setValue( ui->m_openglwidget->getSelectedLight()->model().getRotation().x );
    ui->y_rotation->setValue( ui->m_openglwidget->getSelectedLight()->model().getRotation().y );
    ui->z_rotation->setValue( ui->m_openglwidget->getSelectedLight()->model().getRotation().z );
    ui->x_scale->setValue( 1.0 );
    ui->y_scale->setValue( 1.0 );
    ui->z_scale->setValue( 1.0 );

    Light::LightParam settings = ui->m_openglwidget->getSelectedLight()->getParams();
    glm::vec3 color            = ui->m_openglwidget->getSelectedLight()->color();
    Light::LightType type      = ui->m_openglwidget->getSelectedLight()->getType();
    updateSettings( type );
    switch ( type )
    {
    case Light::LightType::POINT_LIGHT:
        ui->object_settings_label->setText( "Point Light Settings" );
        ui->intensitySpin->setValue( settings.pointlight.intensity );
        break;
    case Light::LightType::SPOT_LIGHT:
        ui->object_settings_label->setText( "Spot Light Settings" );
        ui->intensitySpin->setValue( settings.spotlight.intensity );
        ui->innercutSpin->setValue( settings.spotlight.innerCutoff );
        ui->outercutSpin->setValue( settings.spotlight.outerCutoff );
    default:
        break;
    }
    ui->colorRSpin->setValue( int( color.r * 255.f ) );
    ui->colorGSpin->setValue( int( color.g * 255.f ) );
    ui->colorBSpin->setValue( int( color.b * 255.f ) );
}

void MainWindow::on_objectCreator_activated( const QString& arg1 ) {
    if ( arg1 == "UVSphere" )
        ui->m_openglwidget->addObject( Model::UV_SPHERE );
    else if ( arg1 == "IcoSphere" )
        ui->m_openglwidget->addObject( Model::ICO_SPHERE );
    else if ( arg1 == "CubeSphere" )
        ui->m_openglwidget->addObject( Model::CUBE_SPHERE );
    else if ( arg1 == "Cube" )
        ui->m_openglwidget->addObject( Model::CUBE );
    else if ( arg1 == "Particle System")
        ui->m_openglwidget->addObject( Model::PARTICLESYS );
    ui->m_openglwidget->setFocus();
    ui->objectCreator->setCurrentIndex( 0 );
}

void MainWindow::on_lightCreator_activated( const QString& arg1 ) {
    if ( arg1 == "Point Light" )
        ui->m_openglwidget->addLight( Light::POINT_LIGHT );
    else if ( arg1 == "Spot Light" )
        ui->m_openglwidget->addLight( Light::SPOT_LIGHT );
    else if ( arg1 == "Directionnal Light" )
        ui->m_openglwidget->addLight( Light::DIR_LIGHT );
    ui->m_openglwidget->setFocus();
    ui->lightCreator->setCurrentIndex( 0 );
}

void MainWindow::on_shaderChoser_activated( int index ) {
    ui->m_openglwidget->setShader( GLuint( index ) );
    ui->m_openglwidget->setFocus();
}

void MainWindow::on_uv_meridians_valueChanged( int arg1 ) {
    if ( m_state == DaftState::SELECTION ) return;
    Model::ModelParam settings{};
    settings.uv_sphere.meridians = ui->uv_meridians->value();
    settings.uv_sphere.parallels = ui->uv_parallels->value();
    ui->m_openglwidget->setModelParams( settings );
    ui->m_openglwidget->update();
}

void MainWindow::on_uv_parallels_valueChanged( int arg1 ) {
    on_uv_meridians_valueChanged( arg1 );
}

void MainWindow::on_ico_subdiv_valueChanged( int arg1 ) {
    if ( m_state == DaftState::SELECTION ) return;
    Model::ModelParam settings{};
    settings.ico_sphere.subdivisions = ui->ico_subdiv->value();
    ui->m_openglwidget->setModelParams( settings );
    ui->m_openglwidget->update();
}

void MainWindow::on_cube_res_valueChanged( int arg1 ) {
    if ( m_state == DaftState::SELECTION ) return;
    Model::ModelParam settings{};
    settings.cube_sphere.resolution = ui->cube_res->value();
    ui->m_openglwidget->setModelParams( settings );
    ui->m_openglwidget->update();
}

void MainWindow::on_cubec_res_valueChanged( int arg1 ) {
    if ( m_state == DaftState::SELECTION ) return;
    Model::ModelParam settings{};
    settings.cube.resolution = arg1;
    ui->m_openglwidget->setModelParams( settings );
    ui->m_openglwidget->update();
}

void MainWindow::updateSettings( Model::ModelType type ) {
    switch ( type )
    {
    case Model::ModelType::UV_SPHERE:
        ui->uv_meridians->setVisible( true );
        ui->uv_parallels->setVisible( true );
        ui->uv_meridians_label->setVisible( true );
        ui->uv_parallels_label->setVisible( true );
        break;
    case Model::ModelType::ICO_SPHERE:
        ui->ico_subdiv->setVisible( true );
        ui->ico_subdiv_label->setVisible( true );
        break;
    case Model::ModelType::CUBE_SPHERE:
        ui->cube_res->setVisible( true );
        ui->cube_res_label->setVisible( true );
        break;
    case Model::ModelType::CUBE:
        ui->cubec_res->setVisible( true );
        ui->cubec_res_label->setVisible( true );
    default:
        break;
    }
    ui->triangle_num->setVisible( true );
}

void MainWindow::updateSettings( Light::LightType type ) {
    ui->colorLabel->setVisible( true );
    ui->colorRSpin->setVisible( true );
    ui->colorGSpin->setVisible( true );
    ui->colorBSpin->setVisible( true );
    switch ( type )
    {
    case Light::LightType::POINT_LIGHT:
        ui->intensitySpin->setVisible( true );
        ui->intensityLabel->setVisible( true );
        break;
    case Light::LightType::SPOT_LIGHT:
        ui->intensitySpin->setVisible( true );
        ui->intensityLabel->setVisible( true );
        ui->innercutSpin->setVisible( true );
        ui->innercutLabel->setVisible( true );
        ui->outercutSpin->setVisible( true );
        ui->outercutLabel->setVisible( true );
    default:
        break;
    }
}

void MainWindow::unset_settings() {
    ui->x_position->setValue( 0.0 );
    ui->y_position->setValue( 0.0 );
    ui->z_position->setValue( 0.0 );
    ui->x_rotation->setValue( 0.0 );
    ui->y_rotation->setValue( 0.0 );
    ui->z_rotation->setValue( 0.0 );
    ui->x_scale->setValue( 1.0 );
    ui->y_scale->setValue( 1.0 );
    ui->z_scale->setValue( 1.0 );
    // object settings
    ui->object_settings_label->setText( "Model Settings" );
    ui->uv_meridians->setVisible( false );
    ui->uv_parallels->setVisible( false );
    ui->uv_meridians_label->setVisible( false );
    ui->uv_parallels_label->setVisible( false );
    ui->ico_subdiv->setVisible( false );
    ui->ico_subdiv_label->setVisible( false );
    ui->cube_res->setVisible( false );
    ui->cube_res_label->setVisible( false );
    ui->cubec_res->setVisible( false );
    ui->cubec_res_label->setVisible( false );
    ui->triangle_num->setVisible( false );
    // light settings
    ui->intensitySpin->setVisible( false );
    ui->intensityLabel->setVisible( false );
    ui->colorLabel->setVisible( false );
    ui->colorRSpin->setVisible( false );
    ui->colorGSpin->setVisible( false );
    ui->colorBSpin->setVisible( false );
    ui->innercutSpin->setVisible( false );
    ui->innercutLabel->setVisible( false );
    ui->outercutSpin->setVisible( false );
    ui->outercutLabel->setVisible( false );
    // material settings
    ui->materialSettingsWidget->setEnabled( false );
}

void MainWindow::on_deleteButton_clicked() {
    ui->m_openglwidget->deleteObject();
}

void MainWindow::on_gizmoType_activated( const QString& arg1 ) {
    if ( arg1 == "Translation" ) { ui->m_openglwidget->setGizmoType( Gizmo::TRANSLATE ); }
    else if ( arg1 == "Scale" )
    { ui->m_openglwidget->setGizmoType( Gizmo::SCALE ); }
}

void MainWindow::on_editionToggle_clicked() {
    ui->m_openglwidget->toggleEditionMode();
    ui->m_openglwidget->setFocus();
    ui->m_openglwidget->update();
}

void MainWindow::on_colorSpin_valueChanged() {
    if ( m_state == SELECTION ) return;
    glm::vec3 new_color{ui->colorRSpin->value() / 255.f,
                        ui->colorGSpin->value() / 255.f,
                        ui->colorBSpin->value() / 255.f};
    if ( ui->m_openglwidget->getSelectedLight() != nullptr )
        ui->m_openglwidget->getSelectedLight()->color() = new_color;
    ui->m_openglwidget->update();
}

void MainWindow::on_innercutSpin_valueChanged( double arg1 ) {
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedLight() == nullptr ) return;
    Light::LightParam params = ui->m_openglwidget->getSelectedLight()->getParams();
    if ( ui->m_openglwidget->getSelectedLight()->getType() == Light::LightType::SPOT_LIGHT )
    {
        if ( float( arg1 ) > params.spotlight.outerCutoff )
        {
            ui->innercutSpin->setValue( params.spotlight.innerCutoff );
            return;
        }
        params.spotlight.innerCutoff = float( arg1 );
        ui->m_openglwidget->getSelectedLight()->editLight( params );
    }
    ui->m_openglwidget->update();
}

void MainWindow::on_outercutSpin_valueChanged( double arg1 ) {
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedLight() == nullptr ) return;
    Light::LightParam params = ui->m_openglwidget->getSelectedLight()->getParams();
    if ( ui->m_openglwidget->getSelectedLight()->getType() == Light::LightType::SPOT_LIGHT )
    {
        if ( float( arg1 ) < params.spotlight.innerCutoff )
        {
            ui->outercutSpin->setValue( params.spotlight.outerCutoff );
            return;
        }
        params.spotlight.outerCutoff = float( arg1 );
        ui->m_openglwidget->getSelectedLight()->editLight( params );
    }
    ui->m_openglwidget->update();
}

void MainWindow::on_intensitySpin_valueChanged( double arg1 ) {
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedLight() == nullptr ) return;
    Light::LightParam params = ui->m_openglwidget->getSelectedLight()->getParams();
    switch ( ui->m_openglwidget->getSelectedLight()->getType() )
    {
    case Light::POINT_LIGHT:
        params.pointlight.intensity = float( arg1 );
        break;
    case Light::SPOT_LIGHT:
        params.spotlight.intensity = float( arg1 );
        break;
    default:
        break;
    }
    ui->m_openglwidget->getSelectedLight()->editLight( params );
    ui->m_openglwidget->update();
}

void MainWindow::on_albedoSpin_valueChanged() {
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedObject() == nullptr ) return;
    glm::vec3 new_albedo{ui->albedoRspin->value() / 255.f,
                         ui->albedoGspin->value() / 255.f,
                         ui->albedoBspin->value() / 255.f};
    ui->m_openglwidget->getSelectedObject()->material().albedo() = new_albedo;
    ui->m_openglwidget->update();
}

void MainWindow::on_specularSpin_valueChanged() {
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedObject() == nullptr ) return;
    glm::vec3 new_specular{ui->specularRspin->value() / 255.f,
                           ui->specularGspin->value() / 255.f,
                           ui->specularBspin->value() / 255.f};
    ui->m_openglwidget->getSelectedObject()->material().specular() = new_specular;
    ui->m_openglwidget->update();
}

void MainWindow::on_roughnessSpin_valueChanged( double arg1 ) {
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedObject() == nullptr ) return;
    ui->m_openglwidget->getSelectedObject()->material().roughness() = float( arg1 );
    ui->m_openglwidget->update();
}

void MainWindow::on_aoSpin_valueChanged( double arg1 ) {
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedObject() == nullptr ) return;
    ui->m_openglwidget->getSelectedObject()->material().ambientOcclusion() = float( arg1 );
    ui->m_openglwidget->update();
}

void MainWindow::on_iorSpin_valueChanged( double arg1 ) {
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedObject() == nullptr ) return;
    ui->m_openglwidget->getSelectedObject()->material().IOR() = float( arg1 );
    ui->m_openglwidget->update();
}

void MainWindow::on_trianglesChanged( GLuint size ) {
    if ( m_state == SELECTION ) return;
    ui->triangle_num->setText( ( "Number of triangles : " + std::to_string( size ) ).c_str() );
}

void MainWindow::on_renderButton_clicked()
{
    std::string path = "output/" + std::string(ui->picturename->text().toUtf8().constData());
    ui->picturename->setText("");
    ui->m_openglwidget->raytrace(path);
}

void MainWindow::on_transSpin_valueChanged(double arg1)
{
    if ( m_state == SELECTION ) return;
    if ( ui->m_openglwidget->getSelectedObject() == nullptr )   return;
    ui->m_openglwidget->getSelectedObject()->material().transparency() = float( arg1 );
    ui->m_openglwidget->update();
}
