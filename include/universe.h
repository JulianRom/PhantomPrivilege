#ifndef BEEF_PLANET_UNIVERSE_H
#define BEEF_PLANET_UNIVERSE_H

#include "game_object.h"
#include "gravity_simulator.h"

#include "delta.h"

#include <vector>

namespace bp {

	class Universe {
	public:
		Universe();
		~Universe();

		void initialize(dbasic::DeltaEngine* engine);

		void process(float dt);
		void render();

		template<typename T_GameObject>
		T_GameObject* spawn() {
			T_GameObject* newObject = new T_GameObject;
			newObject->initialize(m_engine, this);

			m_spawn_queue.push_back(newObject);

			return newObject;
		}

		GravitySimulator* getGravitySimulator() { return &m_gravity_simulator; }

		int getGameObjectCount() const { return m_game_objects.size(); }

	private:
		void processSpawnQueue();
		void worldDestroyer();
		GravitySimulator m_gravity_simulator;

	private:
		dbasic::DeltaEngine* m_engine;
		std::vector<GameObject *> m_spawn_queue;
		std::vector<GameObject *> m_game_objects;
	};

} /* namespace bp */

#endif /* BEEF_PLANET_UNIVERSE_H */
