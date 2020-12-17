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

    m_engine.SetClearColor(ysColor::srgbiToLinear(0x22, 0x00, 0x44));

    m_assetManager.SetEngine(&m_engine);

    err = m_assetManager.CompileInterchangeFile((assetPath + "/icosphere").c_str(), 1.0f, true);
    m_assetManager.LoadSceneFile((assetPath + "/icosphere").c_str(), true);

    m_engine.SetCameraMode(dbasic::DeltaEngine::CameraMode::Target);
    m_engine.SetCameraPosition(ysMath::Constants::Zero);
    m_engine.SetCameraTarget(ysMath::Constants::Zero);

    m_engine.SetCursorPositionLock(true);
    m_engine.SetCursorHidden(false);

    m_universe.initialize(&m_engine);

    PlanetSpawner *planet_spawner = m_universe.spawn<PlanetSpawner>();
    planet_spawner->setModel(m_assetManager.GetModelAsset("Icosphere"));

    m_player = m_universe.spawn<PlayerObject>();
    m_player->getPhysicsComponent()->m_transform.SetPosition(ysMath::LoadVector(0.0f, 0.0f, 0.0f));
    m_player->setModel(m_assetManager.GetModelAsset("Icosphere"));
}

void bp::BeefPlanet::Process() {
    m_universe.process(m_engine.GetFrameLength());
}

void bp::BeefPlanet::Render() {
    m_engine.ResetLights();
    m_engine.SetAmbientLight(ysMath::GetVector4(ysColor::srgbiToLinear(0x33, 0x00, 0x55)));

    dbasic::Light glow;
    glow.Active = 1;
    glow.Attenuation0 = 0.0f;
    glow.Attenuation1 = 0.0f;
    glow.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0xf1, 0xc4, 0x0f), ysMath::LoadScalar(50.0f)));
    glow.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    glow.FalloffEnabled = 1;
    glow.Position = ysMath::GetVector4(m_player->getPhysicsComponent()->m_transform.GetWorldPosition());
    m_engine.AddLight(glow);
    
    m_engine.SetSpecularRoughness(0.5f);

    m_universe.render();

    m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0f, nullptr);
    m_engine.SetDrawTarget(dbasic::DeltaEngine::DrawTarget::Gui);
    m_engine.SetObjectTransform(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, 0.0f)));
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0xAA, 0x00, 0x99));
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
    ss << m_universe.getGameObjectCount();

    m_engine.GetConsole()->DrawGeneralText(ss.str().c_str());
}

void bp::BeefPlanet::Run() {
    while (m_engine.IsOpen() && !m_engine.IsKeyDown(ysKeyboard::KEY_ESCAPE)) {
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
