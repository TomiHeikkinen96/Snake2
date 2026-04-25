#pragma once

namespace Snake2 {

class Game;

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void update(Game& game, float deltaSeconds) = 0;
};

} // namespace Snake2
