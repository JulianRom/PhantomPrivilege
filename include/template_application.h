#ifndef DELTA_TEMPLATE_TEMPLATE_APPLICATION_H
#define DELTA_TEMPLATE_TEMPLATE_APPLICATION_H

#include "delta.h"
#include "mouse_controller.h"

class TemplateApplication {
public:
    TemplateApplication();
    ~TemplateApplication();

    void Initialize(void *instance, ysContextObject::DeviceAPI api);
    void Run();
    void Destroy();

protected:
    void Process();
    void Render();

    void DrawDebugScreen();

    dbasic::DeltaEngine m_engine;
    dbasic::AssetManager m_assetManager;

    ysTexture *m_demoTexture;

    float m_currentRotation;
    float m_glow;
    ysVector m_planet_position;

    MouseController m_mouse_controller;
};

#endif /* DELTA_TEMPLATE_TEMPLATE_APPLICATION_H */
