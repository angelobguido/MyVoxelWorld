cmake_minimum_required(VERSION 3.28)
project(MyVoxelWorld)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyVoxelWorld
        main.cpp
        source/ShaderProgram.cpp
        source/App.cpp
        source/Renderer.cpp
        source/Renderer.h
        source/Camera.cpp
        source/Camera.h
        source/VoxelWorldBuilder.cpp
        source/VoxelWorldBuilder.h
        source/PerlinNoise/PerlinNoise.hpp
        source/Detector.cpp
        source/Detector.h
        source/Saver.cpp
        source/Saver.h
)

target_link_libraries(MyVoxelWorld -lglfw3 -lglew32 -lopengl32)