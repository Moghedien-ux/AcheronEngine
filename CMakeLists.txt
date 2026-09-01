cmake_minimum_required(VERSION 3.20)

project(DX3DGameEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Unicode / UTF-8
add_definitions(-DUNICODE -D_UNICODE)

# ---------------------------------------------------------------------------
# Setup ImGui from local directory (imgui)
# ---------------------------------------------------------------------------
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

if(EXISTS "${IMGUI_DIR}/imgui.h")
    message(STATUS "Found local ImGui in: ${IMGUI_DIR}")

    file(GLOB IMGUI_SOURCES
            "${IMGUI_DIR}/*.cpp"
            "${IMGUI_DIR}/backends/imgui_impl_win32.cpp"
            "${IMGUI_DIR}/backends/imgui_impl_dx11.cpp"
    )

    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC
            "${IMGUI_DIR}"
            "${IMGUI_DIR}/backends"
    )
else()
    message(STATUS "Local ImGui not found in ${IMGUI_DIR}, fetching via FetchContent...")
    include(FetchContent)

    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG        v1.90.4
    )
    FetchContent_MakeAvailable(imgui)
endif()

# Include Directories for Engine
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Include"
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Source"
        "${IMGUI_DIR}"
        "${IMGUI_DIR}/backends"
)

# Engine Core, Math, Geometry, Scene, Physics, Graphics & UI Sources
file(GLOB_RECURSE SOURCES_ENGINE
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Include/*.h"
)

# Shader files (for IDE grouping in CLion and Visual Studio)
file(GLOB_RECURSE SHADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Shaders/*.hlsl"
)

# Main Executable
add_executable(${PROJECT_NAME}
        "${CMAKE_CURRENT_SOURCE_DIR}/Game/main.cpp"
        ${SOURCES_ENGINE}
        ${SHADERS}
)

# Link DirectX 11 Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        imgui
        d3d11
        dxgi
        d3dcompiler
        user32
        gdi32
        kernel32
        dxguid
)

# Set working directory to project root so shaders are loaded properly
set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Compiler warnings and options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /Zc:__cplusplus)
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:CONSOLE")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy Shaders to build output directory automatically on build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/DX3D/Shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders"
)