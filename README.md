# Snake2

Snake2 is a compact C++17/SDL2 learning project. It renders a simple Snake game with rectangles while keeping the code organized around RAII, smart pointers, a small ECS, systems, plugins, and events.

## Build

### Visual Studio 2026

This project uses CMake and SDL2. The repository includes `vcpkg.json`, so Visual Studio/vcpkg can restore SDL2 automatically.

1. Open Visual Studio.
2. Choose **File > Open > Folder...**.
3. Open this repository folder.
4. Configure the CMake project for x64.
5. Build and run the `Snake2` target.

On Windows, CMake will try to find vcpkg automatically from either `VCPKG_ROOT` or Visual Studio's bundled vcpkg installation. If it still cannot find SDL2, set `CMAKE_TOOLCHAIN_FILE` to your vcpkg toolchain file:

```text
C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Command Line

From a Visual Studio Developer PowerShell or Developer Command Prompt:

```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Debug
```

If SDL2 is already available through your system CMake packages, this may also work:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

## Controls

- Arrow keys or WASD: move
- Escape: quit

## Architecture Map

- `Game` owns the core managers with `std::unique_ptr`.
- `EntityManager` stores entities by ID, and entities store components with `std::unique_ptr<IComponent>`.
- `SystemManager` owns ordered systems: input, movement, collision, render.
- `EventBus` publishes typed events and returns RAII subscription tokens.
- `ResourceManager` demonstrates `std::shared_ptr` texture handles and a `std::weak_ptr` back-reference to avoid a shared ownership cycle.
- Plugins are `std::unique_ptr` owned by `Game`; `ScorePlugin` listens to food/game-over events, and `SaveLoadPlugin` persists a high score through RAII file streams.

## Learning TODOs

Search for `TODO(learning)` in the codebase. They point at ownership experiments, raw-pointer replacement opportunities, shared pointer cycle risks, system/component extensions, and plugin behavior ideas.

This is intentionally small rather than engine-perfect. The code aims to be readable enough to take apart and reshape.
