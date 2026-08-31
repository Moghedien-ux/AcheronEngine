#include <DX3D/All.h>
#include <iostream>
#include <Windows.h>

int main()
{
    try
    {
        dx3d::Game game({ { 1280, 720 }, dx3d::Logger::LogLevel::Info });
        game.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "[DX3D Error]: " << e.what() << std::endl;
        MessageBoxA(nullptr, e.what(), "DX3D Runtime Error", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "[DX3D Fatal Error]: Unknown exception." << std::endl;
        MessageBoxA(nullptr, "An unknown error occurred.", "DX3D Fatal Error", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}