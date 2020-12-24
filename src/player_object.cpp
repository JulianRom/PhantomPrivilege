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
    m_color = ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF);
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
        movement = ysMath::Mul(movement, ysMath::LoadScalar(1000));
        getPhysicsComponent()->forceAdd(movement);
    }
    else if (m_engine->IsKeyDown(ysKey::Code::S)) {
        ysVector movement;
        movement = ysMath::Sub(m_engine->GetCameraTarget(), m_engine->GetCameraPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(movement, ysMath::LoadScalar(1000));
        getPhysicsComponent()->forceAdd(ysMath::Negate(movement));
    }

    m_mouse_controller.process(getPhysicsComponent()->m_transform.GetWorldPosition());
}

void bp::PlayerObject::render()
{
    m_engine->ResetBrdfParameters();
    m_engine->SetBaseColor(m_color);
    m_engine->SetObjectTransform(m_physics_component.m_transform.GetWorldTransform());
    m_engine->SetSpecularRoughness(1.0f);
    //m_engine->SetEmission(ysMath::Mul(ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF), ysMath::LoadScalar(0.2f)));
    m_engine->DrawModel(m_model, m_size, nullptr);

    dbasic::Light glow;
    glow.Active = 1;
    glow.Attenuation0 = 0.0f;
    glow.Attenuation1 = 0.0f;
    glow.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF), ysMath::LoadScalar(50.0f)));
    glow.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    glow.FalloffEnabled = 1;
    glow.Position = ysMath::GetVector4(getPhysicsComponent()->m_transform.GetWorldPosition());
    m_engine->AddLight(glow);
}

void bp::PlayerObject::updateMass(float mass)
{
    PhysicalObject::updateMass(mass);
    m_mouse_controller.setZoom(m_size * 7);
}
