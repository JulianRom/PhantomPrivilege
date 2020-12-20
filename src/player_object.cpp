#include "..\include\player_object.h"
#include "..\include\mouse_controller.h"

bp::PlayerObject::PlayerObject()
{
}

bp::PlayerObject::~PlayerObject()
{
}

void bp::PlayerObject::initialize(dbasic::DeltaEngine* engine, Universe* universe)
{
	PhysicalObject::initialize(engine, universe);
    m_mouse_controller.initialize(engine);
    updateMass(50.0);
}

void bp::PlayerObject::process(float dt)
{
    if (m_engine->IsKeyDown(ysKey::Code::A)) {
		updateMass(50.0);
    }
    else if (m_engine->IsKeyDown(ysKey::Code::D)) {
		updateMass(10000.0);
    }

    if (m_engine->IsKeyDown(ysKey::Code::W)) {
        ysVector movement;
        movement = ysMath::Sub(m_engine->GetCameraTarget(), m_engine->GetCameraPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(movement, ysMath::LoadScalar(6 * m_engine->GetFrameLength()));
        getPhysicsComponent()->m_transform.SetPosition(ysMath::Add(getPhysicsComponent()->m_transform.GetWorldPosition(), movement));
    }
    else if (m_engine->IsKeyDown(ysKey::Code::S)) {
        ysVector movement;
        movement = ysMath::Sub(m_engine->GetCameraTarget(), m_engine->GetCameraPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(movement, ysMath::LoadScalar(6 * m_engine->GetFrameLength()));
        getPhysicsComponent()->m_transform.SetPosition(ysMath::Sub(getPhysicsComponent()->m_transform.GetWorldPosition(), movement));
    }

    m_mouse_controller.process(getPhysicsComponent()->m_transform.GetWorldPosition());
}

void bp::PlayerObject::render()
{
	m_engine->ResetBrdfParameters();
	m_engine->SetBaseColor(m_color);
	m_engine->SetObjectTransform(m_physics_component.m_transform.GetWorldTransform());
	m_engine->SetEmission(ysMath::Mul(ysColor::srgbiToLinear(0xf1, 0xc4, 0x0f), ysMath::LoadScalar(5.0f)));
	m_engine->DrawModel(m_model, m_size, nullptr);
}

void bp::PlayerObject::updateMass(float mass)
{
	PhysicalObject::updateMass(mass);
    m_mouse_controller.setZoom(m_size * 7);
}
