cmake_minimum_required(VERSION 3.28)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyVoxelWorld
        main.cpp
        source/ShaderProgram.cpp
        source/App.cpp
        source/Renderer.cpp
        source/Renderer.h
        source/Camera.cpp
        source/Camera.h
)

target_link_libraries(MyVoxelWorld -lglfw -lGL -lGLEW -lm)