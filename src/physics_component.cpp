#include "..\include\physics_component.h"

bp::PhysicsComponent::PhysicsComponent()
{
    m_velocity = ysMath::Constants::Zero;
    m_inverse_mass = 1.0;
    m_parent = nullptr;
    m_force_accumulator = ysMath::Constants::Zero;
}

bp::PhysicsComponent::~PhysicsComponent()
{
}

void bp::PhysicsComponent::integrate(float dt)
{
    ysVector acceleration;
    ysVector inverse_mass = ysMath::LoadScalar(m_inverse_mass);
    ysVector time_step = ysMath::LoadScalar(dt);
    ysVector position = m_transform.GetPositionParentSpace();
    ysVector limiter = ysMath::LoadVector(1000.0, 1000.0, 1000.0, 1000.0);

    acceleration = ysMath::Mul(ysMath::ComponentMax(ysMath::ComponentMin(m_force_accumulator, limiter), ysMath::Negate(limiter)), inverse_mass);
    m_velocity = ysMath::Add(m_velocity, ysMath::Mul(acceleration, time_step));
    m_transform.SetPosition(ysMath::Add(position, ysMath::Mul(m_velocity, time_step)));
}

void bp::PhysicsComponent::clearIntersection()
{
    m_intersections.clear();
}

void bp::PhysicsComponent::addIntersection(PhysicalObject* secondObject)
{
    m_intersections.push_back(secondObject);
}
