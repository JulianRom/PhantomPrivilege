#ifndef BEEF_PLANET_TEMPLATE_APPLICATION_H
#define BEEF_PLANET_TEMPLATE_APPLICATION_H

#include "delta.h"
#include "mouse_controller.h"
#include "universe.h"
#include "player_object.h"

namespace bp {
    class BeefPlanet {
    public:
        BeefPlanet();
        ~BeefPlanet();

        void Initialize(void* instance, ysContextObject::DeviceAPI api);
        void Run();
        void Destroy();

    protected:
        void Process();
        void Render();

        void DrawDebugScreen();

        dbasic::DeltaEngine m_engine;
        dbasic::AssetManager m_assetManager;

        PlayerObject *m_player;

        Universe m_universe;

    };
}  /* namespace bp */
#endif /* BEEF_PLANET_TEMPLATE_APPLICATION_H */
