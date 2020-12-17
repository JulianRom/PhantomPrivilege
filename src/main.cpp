#include "../include/beef_planet.h"

#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)nCmdShow;
    (void)lpCmdLine;
    (void)hPrevInstance;

    bp::BeefPlanet app;
    app.Initialize((void *)&hInstance, ysContextObject::DeviceAPI::OpenGL4_0); 
    app.Run();
    app.Destroy();

    return 0;
}
