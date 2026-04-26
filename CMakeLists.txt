cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(_snake2_vcpkg_toolchain "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

    if(EXISTS "${_snake2_vcpkg_toolchain}")
        set(CMAKE_TOOLCHAIN_FILE "${_snake2_vcpkg_toolchain}" CACHE FILEPATH "vcpkg toolchain file")
    elseif(WIN32)
        set(_snake2_vswhere "$ENV{ProgramFiles(x86)}/Microsoft Visual Studio/Installer/vswhere.exe")

        if(EXISTS "${_snake2_vswhere}")
            execute_process(
                COMMAND "${_snake2_vswhere}" -latest -products * -property installationPath
                OUTPUT_VARIABLE _snake2_vs_install_path
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )

            set(_snake2_vcpkg_toolchain "${_snake2_vs_install_path}/VC/vcpkg/scripts/buildsystems/vcpkg.cmake")
            if(EXISTS "${_snake2_vcpkg_toolchain}")
                set(CMAKE_TOOLCHAIN_FILE "${_snake2_vcpkg_toolchain}" CACHE FILEPATH "vcpkg toolchain file")
            endif()
        endif()
    endif()
endif()

project(Snake2 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(Snake2Core
    src/Core/SDLWrappers.cpp
    src/ECS/EntityManager.cpp
    src/ECS/SystemManager.cpp
    src/ECS/Systems.cpp
    src/Game.cpp
    src/Plugins/SaveLoadPlugin.cpp
    src/Plugins/ScorePlugin.cpp
    src/Resources/ResourceManager.cpp
)

target_include_directories(Snake2Core PUBLIC include)

find_package(SDL2 CONFIG QUIET)

if(TARGET SDL2::SDL2)
    target_link_libraries(Snake2Core PUBLIC SDL2::SDL2)
else()
    find_package(SDL2 REQUIRED)
    target_include_directories(Snake2Core PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(Snake2Core PUBLIC ${SDL2_LIBRARIES})
endif()

add_executable(Snake2 src/main.cpp)
target_link_libraries(Snake2 PRIVATE Snake2Core)

if(TARGET SDL2::SDL2main)
    target_link_libraries(Snake2 PRIVATE SDL2::SDL2main)
endif()

if(MSVC)
    target_compile_options(Snake2Core PRIVATE /W4 /permissive-)
    target_compile_options(Snake2 PRIVATE /W4 /permissive-)
else()
    target_compile_options(Snake2Core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(Snake2 PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)

if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)

    add_executable(Snake2Tests
        tests/Snake2LearningTests.cpp
    )

    target_link_libraries(Snake2Tests PRIVATE
        Snake2Core
        GTest::gtest_main
    )

    if(MSVC)
        target_compile_options(Snake2Tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(Snake2Tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    gtest_discover_tests(Snake2Tests)
endif()
