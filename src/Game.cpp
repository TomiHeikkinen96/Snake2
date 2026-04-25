#include "Snake2/Game.hpp"

#include "Snake2/ECS/Components.hpp"
#include "Snake2/ECS/Systems.hpp"
#include "Snake2/Plugins/SaveLoadPlugin.hpp"
#include "Snake2/Plugins/ScorePlugin.hpp"

#include <SDL.h>

#include <algorithm>
#include <chrono>
#include <iterator>
#include <stdexcept>

namespace Snake2 {

Game::Game()
    : sdl_(std::make_unique<SDLContext>()),
      window_(std::make_unique<Window>("Snake2", GridWidth * CellSize, GridHeight * CellSize)),
      renderer_(std::make_unique<Renderer>(*window_)),
      entities_(std::make_unique<EntityManager>()),
      systems_(std::make_unique<SystemManager>()),
      resources_(std::make_unique<ResourceManager>(renderer())),
      rng_(std::random_device{}()) {
    addPlugin(std::make_unique<ScorePlugin>());
    addPlugin(std::make_unique<SaveLoadPlugin>("snake2_highscore.txt"));

    systems_->addSystem<InputSystem>();
    systems_->addSystem<MovementSystem>(0.11F);
    systems_->addSystem<CollisionSystem>();
    systems_->addSystem<RenderSystem>();

    reset();
}

void Game::run() {
    using clock = std::chrono::steady_clock;
    auto previous = clock::now();

    while (running_) {
        const auto now = clock::now();
        const auto delta = std::chrono::duration<float>(now - previous).count();
        previous = now;

        movedThisFrame_ = false;
        systems_->update(*this, std::min(delta, 0.05F));
    }
}

void Game::requestQuit() {
    running_ = false;
}

bool Game::isRunning() const {
    return running_;
}

EntityManager& Game::entities() {
    return *entities_;
}

const EntityManager& Game::entities() const {
    return *entities_;
}

EventBus& Game::events() {
    return eventBus_;
}

ResourceManager& Game::resources() {
    return *resources_;
}

SDL_Renderer* Game::renderer() {
    return renderer_->get();
}

int Game::gridWidth() const {
    return GridWidth;
}

int Game::gridHeight() const {
    return GridHeight;
}

int Game::cellSize() const {
    return CellSize;
}

int Game::score() const {
    return score_;
}

void Game::setDirection(Direction direction) {
    if (!isOpposite(direction, direction_)) {
        pendingDirection_ = direction;
    }
}

void Game::stepSnake() {
    if (snake_.empty()) {
        return;
    }

    direction_ = pendingDirection_;

    auto* oldHead = entities_->get(snake_.front());
    if (oldHead == nullptr) {
        throw std::runtime_error("Snake head entity is missing.");
    }

    if (auto* segment = oldHead->getComponent<SnakeSegment>()) {
        segment->isHead = false;
    }
    if (auto* renderable = oldHead->getComponent<Renderable>()) {
        renderable->color = SDL_Color{47, 154, 78, 255};
    }

    const auto* oldPosition = oldHead->getComponent<Position>();
    createSnakeSegment(oldPosition->x + direction_.dx, oldPosition->y + direction_.dy, true);

    if (growth_ > 0) {
        --growth_;
    } else {
        entities_->destroyEntity(snake_.back());
        snake_.pop_back();
    }

    movedThisFrame_ = true;
}

void Game::handleCollisions() {
    if (!movedThisFrame_ || snake_.empty()) {
        return;
    }

    const auto* head = entities_->get(snake_.front());
    const auto* headPosition = head == nullptr ? nullptr : head->getComponent<Position>();
    if (headPosition == nullptr) {
        return;
    }

    if (headPosition->x < 0 || headPosition->x >= GridWidth || headPosition->y < 0 || headPosition->y >= GridHeight) {
        endGame();
        return;
    }

    const auto collisionWithSelf = std::any_of(std::next(snake_.begin()), snake_.end(), [&](EntityId id) {
        const auto* segment = entities_->get(id);
        const auto* position = segment == nullptr ? nullptr : segment->getComponent<Position>();
        return position != nullptr && position->x == headPosition->x && position->y == headPosition->y;
    });

    if (collisionWithSelf) {
        endGame();
        return;
    }

    auto* food = entities_->get(food_);
    const auto* foodPosition = food == nullptr ? nullptr : food->getComponent<Position>();
    if (foodPosition != nullptr && foodPosition->x == headPosition->x && foodPosition->y == headPosition->y) {
        ++score_;
        growth_ += 3;
        events().publish(FoodEatenEvent{score_});
        entities_->destroyEntity(food_);
        food_ = 0;
        spawnFood();
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer(), 18, 18, 24, 255);
    SDL_RenderClear(renderer());

    for (const auto* entity : entities_->entities()) {
        const auto* position = entity->getComponent<Position>();
        const auto* renderable = entity->getComponent<Renderable>();
        if (position == nullptr || renderable == nullptr) {
            continue;
        }

        SDL_Rect rect{
            position->x * CellSize,
            position->y * CellSize,
            CellSize - 1,
            CellSize - 1,
        };

        SDL_SetRenderDrawColor(
            renderer(),
            renderable->color.r,
            renderable->color.g,
            renderable->color.b,
            renderable->color.a);
        SDL_RenderFillRect(renderer(), &rect);
    }

    SDL_RenderPresent(renderer());
}

void Game::reset() {
    entities_->clear();
    snake_.clear();
    food_ = 0;
    score_ = 0;
    growth_ = 0;
    direction_ = Direction{1, 0};
    pendingDirection_ = direction_;

    const auto startX = GridWidth / 2;
    const auto startY = GridHeight / 2;
    createSnakeSegment(startX, startY, true);
    createSnakeSegment(startX - 1, startY, false);
    createSnakeSegment(startX - 2, startY, false);
    spawnFood();
}

void Game::addPlugin(std::unique_ptr<Plugin> plugin) {
    plugin->onRegister(*this);
    plugins_.push_back(std::move(plugin));
}

bool Game::isOpposite(Direction a, Direction b) const {
    return a.dx == -b.dx && a.dy == -b.dy;
}

void Game::createSnakeSegment(int x, int y, bool isHead) {
    auto& entity = entities_->createEntity();
    entity.addComponent<Position>(x, y);
    entity.addComponent<SnakeSegment>(isHead);
    entity.addComponent<Renderable>(isHead ? SDL_Color{91, 214, 105, 255} : SDL_Color{47, 154, 78, 255});

    if (isHead) {
        snake_.push_front(entity.id());
    } else {
        snake_.push_back(entity.id());
    }

    // TODO(learning): Add components such as Health, Lifetime, or GridCollider
    // and then move this construction logic into a factory.
}

void Game::spawnFood() {
    std::uniform_int_distribution<int> xDistribution(0, GridWidth - 1);
    std::uniform_int_distribution<int> yDistribution(0, GridHeight - 1);

    int x = 0;
    int y = 0;
    do {
        x = xDistribution(rng_);
        y = yDistribution(rng_);
    } while (isSnakeAt(x, y));

    auto& entity = entities_->createEntity();
    entity.addComponent<Position>(x, y);
    entity.addComponent<Food>();
    entity.addComponent<Renderable>(SDL_Color{226, 72, 91, 255});
    food_ = entity.id();
}

bool Game::isSnakeAt(int x, int y) const {
    return std::any_of(snake_.begin(), snake_.end(), [&](EntityId id) {
        const auto* entity = entities_->get(id);
        const auto* position = entity == nullptr ? nullptr : entity->getComponent<Position>();
        return position != nullptr && position->x == x && position->y == y;
    });
}

void Game::endGame() {
    events().publish(GameOverEvent{score_});
    reset();
}

} // namespace Snake2
