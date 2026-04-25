#include "Snake2/ECS/EntityManager.hpp"

namespace Snake2 {

Entity& EntityManager::createEntity() {
    const auto id = nextId_++;
    auto entity = std::make_unique<Entity>(id);
    auto& ref = *entity;
    entities_[id] = std::move(entity);
    return ref;
}

void EntityManager::destroyEntity(EntityId id) {
    entities_.erase(id);
}

void EntityManager::clear() {
    entities_.clear();
}

Entity* EntityManager::get(EntityId id) {
    const auto found = entities_.find(id);
    return found == entities_.end() ? nullptr : found->second.get();
}

const Entity* EntityManager::get(EntityId id) const {
    const auto found = entities_.find(id);
    return found == entities_.end() ? nullptr : found->second.get();
}

std::vector<Entity*> EntityManager::entities() {
    std::vector<Entity*> result;
    result.reserve(entities_.size());
    for (auto& [id, entity] : entities_) {
        (void)id;
        result.push_back(entity.get());
    }
    return result;
}

std::vector<const Entity*> EntityManager::entities() const {
    std::vector<const Entity*> result;
    result.reserve(entities_.size());
    for (const auto& [id, entity] : entities_) {
        (void)id;
        result.push_back(entity.get());
    }
    return result;
}

} // namespace Snake2
