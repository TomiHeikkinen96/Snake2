#pragma once

#include "Snake2/ECS/System.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace Snake2 {

class Game;

class SystemManager {
public:
    template <typename System, typename... Args>
    System& addSystem(Args&&... args) {
        auto system = std::make_unique<System>(std::forward<Args>(args)...);
        auto& ref = *system;
        systems_.push_back(std::move(system));
        return ref;
    }

    void update(Game& game, float deltaSeconds);

private:
    // TODO(learning): Extend this into staged system groups such as input,
    // simulation, collision, and render when the game becomes larger.
    std::vector<std::unique_ptr<ISystem>> systems_;
};

} // namespace Snake2
