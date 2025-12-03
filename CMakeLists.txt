cmake_minimum_required(VERSION 3.15)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RLIMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/raylib)

add_executable(${PROJECT_NAME}
        app/main.cpp
        app/ExtendedWolfram/ExtendedWolframSimulationView.h
        app/ExtendedWolfram/ExtendedWolframSimulationBuffer.h
        app/ExtendedWolfram/ExtendedWolframSimulation.h
        app/ExtendedWolfram/ExtendedWolfram.ixx
        app/SimulationView.h
        app/ProgramConfig.h
        app/Timer.h
        app/SimulationBuffer.h
        app/ExtendedTotalistic/ExtendedTotalistic.ixx
        app/ExtendedTotalistic/ExtendedTotalisticSimulation.h
        app/ExtendedTotalistic/ExtendedTotalisticSimulationBuffer.h
        app/ExtendedTotalistic/ExtendedTotalisticSimulationView.h
        ${RLIMGUI_DIR}/rlImGui.cpp
        app/Application.h
        app/ControlPanelView.h
        app/ApplicationControls.h
        app/ExtendedWolframSettingsView.h
        app/SettingsPanelView.h
        app/SettingsContentView.h
        app/ApplicationModel.h
        app/CameraModel.h
        app/SimulationFlowConfig.h
)

target_include_directories(
        ${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/boost-di/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/raygui/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
)

add_library(imgui
        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_demo.cpp
)

target_include_directories(imgui PUBLIC
        external/imgui
        external/rlImGui
)

target_link_libraries(${PROJECT_NAME} raylib imgui)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl rt X11)
endif()