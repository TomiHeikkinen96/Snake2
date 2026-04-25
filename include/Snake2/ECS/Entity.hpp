#pragma once

#include "Snake2/Core/Events.hpp"
#include "Snake2/ECS/Components.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace Snake2 {

class Entity {
public:
    explicit Entity(EntityId id) : id_(id) {}

    EntityId id() const {
        return id_;
    }

    template <typename Component, typename... Args>
    Component& addComponent(Args&&... args) {
        auto component = std::make_unique<Component>(std::forward<Args>(args)...);
        auto& ref = *component;
        components_[std::type_index(typeid(Component))] = std::move(component);
        return ref;
    }

    template <typename Component>
    Component* getComponent() {
        const auto found = components_.find(std::type_index(typeid(Component)));
        if (found == components_.end()) {
            return nullptr;
        }
        return static_cast<Component*>(found->second.get());
    }

    template <typename Component>
    const Component* getComponent() const {
        const auto found = components_.find(std::type_index(typeid(Component)));
        if (found == components_.end()) {
            return nullptr;
        }
        return static_cast<const Component*>(found->second.get());
    }

private:
    EntityId id_{};

    // Components are owned by their entity through unique_ptr, keeping lifetime
    // obvious while still allowing polymorphic storage.
    std::unordered_map<std::type_index, std::unique_ptr<IComponent>> components_;
};

} // namespace Snake2
