#include "../include/template_application.h"

#include <sstream>

TemplateApplication::TemplateApplication() {
    m_demoTexture = nullptr;
    m_currentRotation = 0.0f;
    m_glow = 0.0f;
}

TemplateApplication::~TemplateApplication() {
    /* void */
}

void TemplateApplication::Initialize(void *instance, ysContextObject::DEVICE_API api) {
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

    m_engine.CreateGameWindow(
        "Delta Template Application",
        instance,
        api,
        (enginePath + "/shaders/").c_str());

    m_engine.SetClearColor(0x11, 0x00, 0x33);

    m_assetManager.SetEngine(&m_engine);

    m_assetManager.LoadTexture((assetPath + "/chicken.png").c_str(), "chicken");
    m_demoTexture = m_assetManager.GetTexture("chicken")->GetTexture();

    ysError err = m_assetManager.CompileInterchangeFile((assetPath + "/icosphere").c_str(), 1.0f, true);
    m_assetManager.LoadSceneFile((assetPath + "/icosphere").c_str(), true);
    int a = 0;

    m_mouse_controller.initialize(&m_engine);
    m_planet_position = ysMath::Constants::Zero;
    m_engine.SetCameraMode(dbasic::DeltaEngine::CameraMode::Target);
    m_engine.SetCameraPosition(ysMath::Constants::Zero);
    m_engine.SetCameraTarget(ysMath::Constants::Zero);

    m_engine.SetCursorPositionLock(true);
    m_engine.SetCursorHidden(false);
}

void TemplateApplication::Process() {
    if (m_engine.IsKeyDown(ysKeyboard::KEY_SPACE)) {
        m_currentRotation += m_engine.GetFrameLength();
    }

    if (m_engine.IsKeyDown(ysKeyboard::KEY_UP)) {
        ysVector movement;
        movement = ysMath::Sub(m_engine.GetCameraTarget(), m_engine.GetCameraPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(movement, ysMath::LoadScalar(6 * m_engine.GetFrameLength()));
        m_planet_position = ysMath::Add(m_planet_position, movement);
    }
    else if (m_engine.IsKeyDown(ysKeyboard::KEY_DOWN)) {
        ysVector movement;
        movement = ysMath::Sub(m_engine.GetCameraTarget(), m_engine.GetCameraPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(movement, ysMath::LoadScalar(6 * m_engine.GetFrameLength()));
        m_planet_position = ysMath::Sub(m_planet_position, movement);
    }

    if (m_engine.IsKeyDown(ysKeyboard::KEY_LEFT)) {
        m_glow -= m_engine.GetFrameLength() * 0.5f;
    }
    else if (m_engine.IsKeyDown(ysKeyboard::KEY_RIGHT)) {
        m_glow += m_engine.GetFrameLength() * 0.5f;
    }

    if (m_glow < 0.0f) m_glow = 0.0f;
    if (m_glow > 1.0f) m_glow = 1.0f;

    m_mouse_controller.process(m_planet_position);
}

void TemplateApplication::Render() {
    m_engine.ResetLights();
    m_engine.SetAmbientLight(ysMath::GetVector4(ysColor::srgbiToLinear(0x11, 0x00, 0x33)));

    dbasic::Light glow;
    glow.Active = 1;
    glow.Attenuation0 = 0.0f;
    glow.Attenuation1 = 0.0f;
    glow.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0xf1, 0xc4, 0x0f), ysMath::LoadScalar(m_glow * 50.0f)));
    glow.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    glow.FalloffEnabled = 1;
    glow.Position = ysMath::GetVector4(m_planet_position);
    m_engine.AddLight(glow);

    ysMatrix rotationTurntable = ysMath::RotationTransform(ysMath::Constants::YAxis, m_currentRotation);
    
    m_engine.SetSpecularRoughness(0.5f);

    m_engine.ResetBrdfParameters();
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0xf1, 0xc4, 0x0f));
    m_engine.SetObjectTransform(ysMath::MatMult(ysMath::TranslationTransform(m_planet_position), rotationTurntable));
    m_engine.SetEmission(ysMath::Mul(ysColor::srgbiToLinear(0xf1, 0xc4, 0x0f), ysMath::LoadScalar(m_glow * 5.0f)));
    m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0f, nullptr);

    m_engine.ResetBrdfParameters();
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0x9b, 0x59, 0xb6));
    m_engine.SetObjectTransform(ysMath::MatMult(ysMath::TranslationTransform(ysMath::LoadVector(3.0f, 0.0f, 0.0f)), rotationTurntable));
    m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0f, nullptr);

    m_engine.ResetBrdfParameters();
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0x34, 0x49, 0x5e));
    m_engine.SetObjectTransform(ysMath::MatMult(ysMath::TranslationTransform(ysMath::LoadVector(-3.0f, 0.0f, 0.0f)), rotationTurntable));
    m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0f, nullptr);

    m_engine.ResetBrdfParameters();
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0xDF, 0x28, 0x23));
    m_engine.SetObjectTransform(ysMath::MatMult(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 3.0f, 0.0f)), rotationTurntable));
    m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0f, nullptr);

    m_engine.ResetBrdfParameters();
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0x7D, 0x09, 0x96));
    m_engine.SetObjectTransform(ysMath::MatMult(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, -3.0f, 0.0f)), rotationTurntable));
    m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0f, nullptr);

    m_engine.ResetBrdfParameters();
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0xEE, 0xC3, 0xA1));
    m_engine.SetObjectTransform(ysMath::MatMult(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, 3.0f)), rotationTurntable));
    m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0f, nullptr);

    m_engine.ResetBrdfParameters();
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0x84, 0xDC, 0xC6));
    m_engine.SetObjectTransform(ysMath::MatMult(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, -3.0f)), rotationTurntable));
    m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0f, nullptr);

    m_engine.SetDrawTarget(dbasic::DeltaEngine::DrawTarget::Gui);
    m_engine.SetObjectTransform(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, 0.0f)));
    m_engine.SetBaseColor(ysColor::srgbiToLinear(0xAA, 0x00, 0x99));
    m_engine.DrawBox(10.0, 10.0, 0);
    m_engine.SetDrawTarget(dbasic::DeltaEngine::DrawTarget::Main);

    DrawDebugScreen();
}

void TemplateApplication::DrawDebugScreen() {
    // Draw debug console output
    std::stringstream ss;
    ss << m_engine.GetAverageFramerate();

    m_engine.GetConsole()->Clear();
    m_engine.GetConsole()->MoveToLocation({ 1, 2 });

    m_engine.GetConsole()->SetFontBold(true);
    m_engine.GetConsole()->DrawGeneralText("/// Debug Console ///\n");

    m_engine.GetConsole()->SetFontBold(false);
    m_engine.GetConsole()->DrawGeneralText("FPS: ");
    m_engine.GetConsole()->SetFontBold(true);
    m_engine.GetConsole()->DrawGeneralText(ss.str().c_str());
}

void TemplateApplication::Run() {
    while (m_engine.IsOpen()) {
        m_engine.StartFrame();

        Process();
        Render();

        m_engine.EndFrame();
    }
}

void TemplateApplication::Destroy() {
    m_assetManager.Destroy();
    m_engine.Destroy();
}
