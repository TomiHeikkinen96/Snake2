#pragma once

#include "Snake2/ECS/System.hpp"

namespace Snake2 {

class InputSystem final : public ISystem {
public:
    void update(Game& game, float deltaSeconds) override;
};

class MovementSystem final : public ISystem {
public:
    explicit MovementSystem(float secondsPerCell);
    void update(Game& game, float deltaSeconds) override;

private:
    float secondsPerCell_{};
    float accumulator_{};
};

class CollisionSystem final : public ISystem {
public:
    void update(Game& game, float deltaSeconds) override;
};

class RenderSystem final : public ISystem {
public:
    void update(Game& game, float deltaSeconds) override;
};

} // namespace Snake2
