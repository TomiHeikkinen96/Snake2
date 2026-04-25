#pragma once

#include "Snake2/Core/EventBus.hpp"
#include "Snake2/Core/Events.hpp"
#include "Snake2/Core/SDLWrappers.hpp"
#include "Snake2/ECS/EntityManager.hpp"
#include "Snake2/ECS/SystemManager.hpp"
#include "Snake2/Plugins/Plugin.hpp"
#include "Snake2/Resources/ResourceManager.hpp"

#include <deque>
#include <memory>
#include <random>
#include <vector>

namespace Snake2 {

struct Direction {
    int dx{};
    int dy{};
};

class Game {
public:
    Game();
    void run();

    void requestQuit();
    bool isRunning() const;

    EntityManager& entities();
    const EntityManager& entities() const;
    EventBus& events();
    ResourceManager& resources();
    SDL_Renderer* renderer();

    int gridWidth() const;
    int gridHeight() const;
    int cellSize() const;
    int score() const;

    void setDirection(Direction direction);
    void stepSnake();
    void handleCollisions();
    void render();
    void reset();
    void addPlugin(std::unique_ptr<Plugin> plugin);

private:
    bool isOpposite(Direction a, Direction b) const;
    void createSnakeSegment(int x, int y, bool isHead);
    void spawnFood();
    bool isSnakeAt(int x, int y) const;
    void endGame();

    static constexpr int GridWidth = 32;
    static constexpr int GridHeight = 24;
    static constexpr int CellSize = 24;

    std::unique_ptr<SDLContext> sdl_;
    std::unique_ptr<Window> window_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<EntityManager> entities_;
    std::unique_ptr<SystemManager> systems_;
    std::unique_ptr<ResourceManager> resources_;

    EventBus eventBus_;

    // Plugins are unique_ptr-owned by Game. They subscribe through RAII tokens,
    // so their event handlers are removed automatically during destruction.
    std::vector<std::unique_ptr<Plugin>> plugins_;

    std::deque<EntityId> snake_;
    EntityId food_{};
    Direction direction_{1, 0};
    Direction pendingDirection_{1, 0};
    bool running_{true};
    bool movedThisFrame_{false};
    int growth_{0};
    int score_{0};

    std::mt19937 rng_;
};

} // namespace Snake2
