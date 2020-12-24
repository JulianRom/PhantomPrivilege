#include "../include/physical_object.h"

#include "../include/universe.h"

#include <math.h>

bp::PhysicalObject::PhysicalObject()
{
    m_model = nullptr;
    m_size = 1.0;
}

bp::PhysicalObject::~PhysicalObject()
{
}

void bp::PhysicalObject::initialize(dbasic::DeltaEngine* engine, Universe* universe)
{
    GameObject::initialize(engine, universe);

    universe->getGravitySimulator()->registerComponent(&m_physics_component);

    m_color = ysColor::srgbiToLinear(ysMath::UniformRandomInt(255), ysMath::UniformRandomInt(255), ysMath::UniformRandomInt(255));
}

void bp::PhysicalObject::process(float dt)
{

}

void bp::PhysicalObject::render()
{
    m_engine->ResetBrdfParameters();
    m_engine->SetBaseColor(m_color);
    m_engine->SetObjectTransform(m_physics_component.m_transform.GetWorldTransform());
    m_engine->SetSpecularRoughness(1.0f);
    m_engine->DrawModel(m_model, m_size, nullptr);
}

void bp::PhysicalObject::updateMass(float mass)
{
    getPhysicsComponent()->setMass(mass);
    m_size = std::pow(m_physics_component.getMass(), 1.0 / 3.0);
}
