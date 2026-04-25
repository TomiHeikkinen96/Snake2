cmake_minimum_required(VERSION 3.16)

project(Snake2 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Snake2
    src/main.cpp
    src/Core/SDLWrappers.cpp
    src/ECS/EntityManager.cpp
    src/ECS/SystemManager.cpp
    src/ECS/Systems.cpp
    src/Game.cpp
    src/Plugins/SaveLoadPlugin.cpp
    src/Plugins/ScorePlugin.cpp
    src/Resources/ResourceManager.cpp
)

target_include_directories(Snake2 PRIVATE include)

find_package(SDL2 CONFIG QUIET)

if(TARGET SDL2::SDL2)
    target_link_libraries(Snake2 PRIVATE SDL2::SDL2)
    if(TARGET SDL2::SDL2main)
        target_link_libraries(Snake2 PRIVATE SDL2::SDL2main)
    endif()
else()
    find_package(SDL2 REQUIRED)
    target_include_directories(Snake2 PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(Snake2 PRIVATE ${SDL2_LIBRARIES})
endif()

if(MSVC)
    target_compile_options(Snake2 PRIVATE /W4 /permissive-)
else()
    target_compile_options(Snake2 PRIVATE -Wall -Wextra -Wpedantic)
endif()
