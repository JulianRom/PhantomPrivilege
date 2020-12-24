#include "../include/beef_planet.h"

#include "../include/physical_object.h"
#include "../include/player_object.h"
#include "../include/planet_spawner.h"

#include <sstream>

bp::BeefPlanet::BeefPlanet() {
    m_player = nullptr;
}

bp::BeefPlanet::~BeefPlanet() {
    /* void */
}

void bp::BeefPlanet::Initialize(void *instance, ysContextObject::DeviceAPI api) {
    dbasic::Path modulePath = dbasic::GetModulePath();
    dbasic::Path confPath = modulePath.Append("delta.conf");

    std::string enginePath = "../../dependencies/delta/engines/basic";
    std::string assetPath = "../../assets";
    if (confPath.Exists()) {
        std::fstream confFile(confPath.ToString(), std::ios::in);
        
        std::getline(confFile, enginePath);
        std::getline(confFile, assetPath);
        enginePath = modulePath.Append(enginePath).ToString();
        assetPath = modulePath.Append(assetPath).ToString();

        confFile.close();
    }

    m_engine.GetConsole()->SetDefaultFontDirectory(enginePath + "/fonts/");

    const std::string shaderPath = (enginePath + "/shaders/").c_str();
    
    dbasic::DeltaEngine::GameEngineSettings settings;
    settings.API = api;
    settings.DepthBuffer = true;
    settings.FrameLogging = false;
    settings.Instance = instance;
    settings.LoggingDirectory = "";
    settings.ShaderDirectory = shaderPath.c_str();
    settings.WindowTitle = "Beef Planet";
    ysError err = m_engine.CreateGameWindow(settings);

    const ysVector clearColor = ysColor::srgbiToLinear(0x22, 0x00, 0x33);
    m_engine.SetClearColor(clearColor);

    m_assetManager.SetEngine(&m_engine);

    err = m_assetManager.CompileInterchangeFile((assetPath + "/icosphere").c_str(), 1.0f, true);
    m_assetManager.LoadSceneFile((assetPath + "/icosphere").c_str(), true);

    m_engine.SetCameraMode(dbasic::DeltaEngine::CameraMode::Target);
    m_engine.SetCameraPosition(ysMath::Constants::Zero);
    m_engine.SetCameraTarget(ysMath::Constants::Zero);

    m_engine.SetCursorPositionLock(true);
    m_engine.SetCursorHidden(true);

    m_universe.initialize(&m_engine);

    PlanetSpawner *planet_spawner = m_universe.spawn<PlanetSpawner>();
    planet_spawner->setModel(m_assetManager.GetModelAsset("Icosphere"));

    m_player = m_universe.spawn<PlayerObject>();
    m_player->getPhysicsComponent()->m_transform.SetPosition(ysMath::LoadVector(0.0f, 0.0f, 0.0f));
    m_player->setModel(m_assetManager.GetModelAsset("Icosphere"));

    m_engine.SetNearClip(1.0f);
    m_engine.SetFarClip(300.0f);

    m_engine.SetFogNear(150.0f);
    m_engine.SetFogFar(300.0f);
    m_engine.SetFogColor(clearColor);
}

void bp::BeefPlanet::Process() {
    m_universe.process(m_engine.GetFrameLength());
}

void bp::BeefPlanet::Render() {
    m_engine.ResetLights();
    m_engine.SetAmbientLight(ysMath::GetVector4(ysColor::srgbiToLinear(0x22, 0x00, 0x33)));

    dbasic::Light sun;
    sun.Active = 1;
    sun.Attenuation0 = 0.0f;
    sun.Attenuation1 = 0.0f;
    sun.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0x33, 0x33, 0x77), ysMath::LoadScalar(1.0f)));
    sun.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    sun.FalloffEnabled = 0;
    sun.Position = ysMath::GetVector4(ysMath::LoadVector(0.0,1000.0,0.0));
    m_engine.AddLight(sun);

    dbasic::Light sun2;
    sun2.Active = 1;
    sun2.Attenuation0 = 0.0f;
    sun2.Attenuation1 = 0.0f;
    sun2.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0x77, 0x33, 0x33), ysMath::LoadScalar(1.0f)));
    sun2.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    sun2.FalloffEnabled = 0;
    sun2.Position = ysMath::GetVector4(ysMath::LoadVector(0.0, -1000.0, 0.0));
    m_engine.AddLight(sun2);

    m_universe.render();

    m_engine.SetDrawTarget(dbasic::DeltaEngine::DrawTarget::Gui);
    m_engine.SetObjectTransform(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, 0.0f)));
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0xFF, 0xFF, 0xFF));
    m_engine.SetLit(false);
    m_engine.DrawBox(10.0, 10.0, 0);
    m_engine.SetDrawTarget(dbasic::DeltaEngine::DrawTarget::Main);

    DrawDebugScreen();
}

void bp::BeefPlanet::DrawDebugScreen() {
    // Draw debug console output
    std::stringstream ss;
    ss << m_engine.GetAverageFramerate() << "\n";

    m_engine.GetConsole()->Clear();
    m_engine.GetConsole()->MoveToOrigin();

    m_engine.GetConsole()->DrawGeneralText("/// Debug Console ///\n");

    m_engine.GetConsole()->DrawGeneralText("FPS: ");
    m_engine.GetConsole()->DrawGeneralText(ss.str().c_str());

    ss = std::stringstream();
    ss << m_universe.getGameObjectCount() << "\n";

    m_engine.GetConsole()->DrawGeneralText(ss.str().c_str());

    ss = std::stringstream();
    ysVector player = m_player->getPhysicsComponent()->m_transform.GetWorldPosition();
    ss << ysMath::GetX(player) << ", " << ysMath::GetY(player) << ", " << ysMath::GetZ(player);

    m_engine.GetConsole()->DrawGeneralText(ss.str().c_str());

}

void bp::BeefPlanet::Run() {
    while (m_engine.IsOpen() && !m_engine.IsKeyDown(ysKey::Code::Escape)) {
        m_engine.StartFrame();

        Process();
        Render();

        m_engine.EndFrame();
    }
}

void bp::BeefPlanet::Destroy() {
    m_assetManager.Destroy();
    m_engine.Destroy();
}
