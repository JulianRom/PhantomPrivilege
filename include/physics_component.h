#ifndef BEEF_PLANET_PHYSICS_COMPONENT_H
#define BEEF_PLANET_PHYSICS_COMPONENT_H

#include "delta.h"

namespace bp {
	class PhysicsComponent {
	public:
		PhysicsComponent();
		~PhysicsComponent();

		void integrate(float dt);

		ysTransform m_transform;
		void setMass(float mass) { m_inverse_mass = 1.0 / mass; }
		void setInverseMass(float inverse_mass) { m_inverse_mass = inverse_mass; }
		float getInverseMass() { return m_inverse_mass; }
		float getMass() { return 1.0 / m_inverse_mass; }
		void forceClear() { m_force_accumulator = ysMath::Constants::Zero; }
		void forceAdd(const ysVector &force) { m_force_accumulator = ysMath::Add(m_force_accumulator, force); }
		ysVector getVelocity() { return m_velocity; }

	private:
		float m_inverse_mass;
		ysVector m_force_accumulator;
		ysVector m_velocity;
	};
} /* namespace bp */

#endif /* BEEF_PLANET_PHYSICS_COMPONENT_H */
