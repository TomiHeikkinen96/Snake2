#include "Snake2/ECS/SystemManager.hpp"

#include "Snake2/Game.hpp"

namespace Snake2 {

void SystemManager::update(Game& game, float deltaSeconds) {
    for (auto& system : systems_) {
        system->update(game, deltaSeconds);
    }
}

} // namespace Snake2
