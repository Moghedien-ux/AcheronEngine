cmake_minimum_required(VERSION 3.20)

# Numele proiectului
project(DirectXGame CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directoare de Include
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Include"
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Source"
)

# Colectarea tuturor fișierelor sursă (.cpp) și header (.h)
file(GLOB_RECURSE SOURCES_CORE
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Source/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Include/*.h"
)

# Fișierul principal de intrare
set(MAIN_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/Game/main.cpp"
)

# Crearea executabilului
add_executable(${PROJECT_NAME}
        ${MAIN_SOURCE}
        ${SOURCES_CORE}
)

# Linkare cu bibliotecile native Windows și DirectX 11
target_link_libraries(${PROJECT_NAME} PRIVATE
        d3d11
        dxgi
        d3dcompiler
        user32
        gdi32
        kernel32
        dxguid
)

# Setare pentru consolă (pentru a vedea std::clog / std::cerr în timpul rulării)
if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:CONSOLE")
endif()

# Copiere automată a shaderelor în directorul de build la compilare (dacă este necesar)
if(EXISTS "DX3D/Shaders")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "DX3D/Shaders"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders"
    )
endif()