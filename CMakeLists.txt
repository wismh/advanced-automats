cmake_minimum_required(VERSION 3.15)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/raylib)

add_executable(${PROJECT_NAME} app/main.cpp
        app/ExtendedWolfram/ExtendedWolframSimulationView.h
        app/ExtendedWolfram/ExtendedWolframSimulationBuffer.h
        app/ExtendedWolfram/ExtendedWolframSimulation.h
        app/ExtendedWolfram/ExtendedWolfram.ixx
        app/SimulationView.h
        app/ProgramConfig.h
        app/Timer.h
        app/SimulationBuffer.h
)

target_include_directories(${PROJECT_NAME} PRIVATE external/boost-di/include)

target_link_libraries(${PROJECT_NAME} raylib)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl rt X11)
endif()