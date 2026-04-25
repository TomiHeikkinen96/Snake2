#include "Snake2/ECS/Systems.hpp"

#include "Snake2/Game.hpp"

#include <SDL.h>

namespace Snake2 {

void InputSystem::update(Game& game, float) {
    SDL_Event event{};
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            game.requestQuit();
            game.events().publish(QuitEvent{});
            continue;
        }

        if (event.type != SDL_KEYDOWN) {
            continue;
        }

        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            game.requestQuit();
            game.events().publish(QuitEvent{});
            break;
        case SDLK_UP:
        case SDLK_w:
            game.setDirection(Direction{0, -1});
            break;
        case SDLK_DOWN:
        case SDLK_s:
            game.setDirection(Direction{0, 1});
            break;
        case SDLK_LEFT:
        case SDLK_a:
            game.setDirection(Direction{-1, 0});
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            game.setDirection(Direction{1, 0});
            break;
        default:
            break;
        }
    }
}

MovementSystem::MovementSystem(float secondsPerCell)
    : secondsPerCell_(secondsPerCell) {}

void MovementSystem::update(Game& game, float deltaSeconds) {
    accumulator_ += deltaSeconds;
    while (accumulator_ >= secondsPerCell_) {
        game.stepSnake();
        accumulator_ -= secondsPerCell_;
    }
}

void CollisionSystem::update(Game& game, float) {
    game.handleCollisions();
}

void RenderSystem::update(Game& game, float) {
    game.render();
}

} // namespace Snake2
