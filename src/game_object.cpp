#include "..\include\game_object.h"

bp::GameObject::GameObject()
{
	m_id = 0;
	m_for_deletion = false;
	m_engine = nullptr;
	m_universe = nullptr;
}

bp::GameObject::~GameObject()
{
}

void bp::GameObject::initialize(dbasic::DeltaEngine* engine, Universe *universe)
{
	m_engine = engine;
	m_universe = universe;
}

void bp::GameObject::process(float dt)
{
}

void bp::GameObject::render()
{
}
