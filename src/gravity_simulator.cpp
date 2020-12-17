#include "..\include\gravity_simulator.h"

#include <assert.h>

bp::GravitySimulator::GravitySimulator()
{

}

bp::GravitySimulator::~GravitySimulator()
{

}

void bp::GravitySimulator::integrate(float dt)
{
	for (PhysicsComponent* component : m_components) {
		component->integrate(dt);
	}
}

void bp::GravitySimulator::generateForces()
{
	for (PhysicsComponent* componentA : m_components) {
		assert(componentA->getInverseMass() != 0);
	}

	for (PhysicsComponent* componentA : m_components) {
		componentA->forceClear();
		for (PhysicsComponent* componentB : m_components) {
			if (componentA == componentB) continue;

			ysVector inverse_mass_mult = ysMath::LoadScalar(-componentA->getInverseMass() * componentB->getInverseMass());
			ysVector combine = ysMath::Sub(componentA->m_transform.GetWorldPosition(), componentB->m_transform.GetWorldPosition());
			ysVector magnitude = ysMath::Magnitude(combine);
			magnitude = ysMath::Mul(magnitude, ysMath::Mul(magnitude, ysMath::Mul(magnitude, inverse_mass_mult)));
			componentA->forceAdd(ysMath::Div(combine, magnitude));
		}
	}
}

void bp::GravitySimulator::registerComponent(PhysicsComponent *component)
{
	m_components.push_back(component);
}

void bp::GravitySimulator::deregisterComponent(PhysicsComponent *component)
{
	int index = -1;
	for (int i = 0; i < m_components.size(); ++i) {
		if (m_components[i] == component) {
			index = i;
			break;
		}
	}
	
	assert(index != -1);

	m_components[index] = m_components.back();
	m_components.resize(m_components.size() - 1);
}
