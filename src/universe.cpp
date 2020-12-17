#include "../include/universe.h"

bp::Universe::Universe()
{
	m_engine = nullptr;
}

bp::Universe::~Universe()
{
}

void bp::Universe::initialize(dbasic::DeltaEngine* engine)
{
	m_engine = engine;
}

void bp::Universe::process(float dt)
{
	worldDestroyer();
	processSpawnQueue();
	m_gravity_simulator.generateForces();
	m_gravity_simulator.integrate(dt);

	for (GameObject* object : m_game_objects) {
		object->process(dt);
	}
}

void bp::Universe::render()
{
	for (GameObject* object : m_game_objects) {
		object->render();
	}
}

void bp::Universe::processSpawnQueue()
{
	for (GameObject* current_spawn : m_spawn_queue) {
		m_game_objects.push_back(current_spawn);
	}
	m_spawn_queue.clear();
}

void bp::Universe::worldDestroyer()
{
	const int gameObjectCount = m_game_objects.size();
	int j = 0;
	for (int i = 0; i < gameObjectCount; ++i) {
		m_game_objects[j] = m_game_objects[i];
		if (!m_game_objects[i]->deletionFlag()) {
			++j;
		}
	}
	m_game_objects.resize(j);
}
