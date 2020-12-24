#ifndef BEEF_PLANET_GRAVITY_SIMULATOR_H
#define BEEF_PLANET_GRAVITY_SIMULATOR_H

#include "physics_component.h"

#include <vector>

namespace bp {

    class GravitySimulator {
    public:
        GravitySimulator();
        ~GravitySimulator();

        void integrate(float dt);
        void generateForces();

        void registerComponent(PhysicsComponent *component);
        void deregisterComponent(PhysicsComponent *component);

    private:
        std::vector<PhysicsComponent *> m_components;
    };

} /* namespace bp */

#endif /* BEEF_PLANET_GRAVITY_SIMULATOR_H */
