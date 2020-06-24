#include "..\include\mouse_controller.h"

MouseController::MouseController()
{
    m_engine = nullptr;
    m_mouse_x = 0;
    m_mouse_y = 0;
    m_pre_x = 0;
    m_pre_y = 0;
    m_zoom = 10;
    m_camera_transform = ysMath::LoadIdentity();
}

MouseController::~MouseController()
{
}

void MouseController::initialize(dbasic::DeltaEngine *engine)
{
    m_engine = engine;
    m_engine->GetMousePos(&m_mouse_x, &m_mouse_y);
    m_pre_x = m_mouse_x;
    m_pre_y = m_mouse_y;
}

void MouseController::process(ysVector &planet_position)
{
    int x;
    int y;
    getMouseChange(&x, &y);
    float rho = (float) y * ysMath::Constants::PI / 2000;
    float phi = (float) x * ysMath::Constants::PI / 2000;

    ysMatrix x_rot = ysMath::RotationTransform(ysMath::Constants::ZAxis, -phi);
    ysMatrix y_rot = ysMath::RotationTransform(ysMath::Constants::XAxis, rho);
    
    ysVector camera_radius = ysMath::LoadVector(0, (float) m_zoom, 0, 0);
    m_camera_transform = ysMath::MatMult(m_camera_transform, x_rot);
    m_camera_transform = ysMath::MatMult(m_camera_transform, y_rot);
    ysVector camera_relative_position = ysMath::MatMult(m_camera_transform, camera_radius);

    ysVector camera_up = ysMath::MatMult(m_camera_transform, ysMath::Constants::ZAxis);

    ysVector camera_position = ysMath::Add(planet_position, camera_relative_position);
    m_engine->SetCameraPosition(camera_position);
    m_engine->SetCameraUp(camera_up);
    m_engine->SetCameraTarget(ysMath::Add(planet_position, ysMath::Mul(camera_up, ysMath::LoadScalar(1.5))));
}

void MouseController::getMouseChange(int *x, int *y)
{
    m_engine->GetMousePos(&m_mouse_x, &m_mouse_y);
    *x = m_mouse_x - m_pre_x;
    *y = m_mouse_y - m_pre_y;
    m_pre_x = m_mouse_x;
    m_pre_y = m_mouse_y;
}

void MouseController::destroy()
{
}
