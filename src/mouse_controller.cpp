#include "..\include\mouse_controller.h"

MouseController::MouseController()
{
    m_engine = nullptr;
    m_mouse_x = 0;
    m_mouse_y = 0;
    m_pre_x = 0;
    m_pre_y = 0;
    m_zoom = 10;
    m_phi = 0.0;
    m_rho = 0.0;
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
    m_phi += (float) x * ysMath::Constants::PI / 2000;
    m_rho += (float) y * ysMath::Constants::PI / 2000;

    ysMatrix x_rot = ysMath::RotationTransform(ysMath::Constants::ZAxis, m_phi);
    ysMatrix y_rot = ysMath::RotationTransform(ysMath::MatMult(x_rot, ysMath::Constants::XAxis), m_rho);
    ysVector camera_radius = ysMath::LoadVector(0, (float) m_zoom, 0, 0);

    ysVector camera_relative_position = ysMath::MatMult(x_rot, camera_radius);
    camera_relative_position = ysMath::MatMult(y_rot, camera_relative_position);

    ysVector camera_up = ysMath::MatMult(y_rot, ysMath::Constants::ZAxis);

    ysVector camera_position = ysMath::Add(planet_position, camera_relative_position);
    m_engine->SetCameraPosition(camera_position);
    m_engine->SetCameraUp(camera_up);
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
