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
    m_gravity_simulator.integrate(dt);
    worldDestroyer();
    processSpawnQueue();

    m_gravity_simulator.clearForces();
    m_gravity_simulator.generateForces();
    
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

void bp::Universe::DrawScaleModel(dbasic::ModelAsset *model, double size, ysTexture *texture, const ysMatrix &transform)
{
    ysMatrix scaledTransform = transform;
    scaledTransform = ysMath::Transpose(scaledTransform);
    scaledTransform.rows[3] = ysMath::Mul(scaledTransform.rows[3], ysMath::LoadVector(m_scale, m_scale, m_scale, 1.0));
    scaledTransform = ysMath::Transpose(scaledTransform);
    m_engine->SetObjectTransform(scaledTransform);
    m_engine->DrawModel(model, size * m_scale, texture);
}

void bp::Universe::addScaleLight(dbasic::Light& light)
{
    light.Position.Scale(m_scale);
    m_engine->AddLight(light);
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
        else {
            m_game_objects[i]->free();
            delete m_game_objects[i];
        }
    }
    m_game_objects.resize(j);
}
