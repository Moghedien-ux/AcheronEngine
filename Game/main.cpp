#include <DX3D/All.h>
#include <iostream>
#include <Windows.h>

int main()
{
    try
    {
        dx3d::GameDesc desc;
        desc.windowSize = { 1440, 850 };
        desc.logLevel = dx3d::Logger::LogLevel::Info;

        dx3d::Game game(desc);
        game.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "[DX3D Fatal Error]: " << e.what() << std::endl;
        MessageBoxA(nullptr, e.what(), "DX3D Engine Error", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "[DX3D Fatal Error]: Unknown exception occurred." << std::endl;
        MessageBoxA(nullptr, "An unknown error occurred.", "DX3D Engine Error", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
