#pragma once

#include "Snake2/ECS/Entity.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace Snake2 {

class EntityManager {
public:
    Entity& createEntity();
    void destroyEntity(EntityId id);
    void clear();

    Entity* get(EntityId id);
    const Entity* get(EntityId id) const;

    std::vector<Entity*> entities();
    std::vector<const Entity*> entities() const;

private:
    EntityId nextId_{1};
    std::unordered_map<EntityId, std::unique_ptr<Entity>> entities_;
};

} // namespace Snake2
