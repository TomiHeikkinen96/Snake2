#pragma once

#include <algorithm>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Snake2 {

class EventBus {
public:
    class Subscription {
    public:
        Subscription() = default;
        Subscription(EventBus& bus, std::type_index type, std::size_t id)
            : bus_(&bus), type_(type), id_(id) {}

        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;

        Subscription(Subscription&& other) noexcept
            : bus_(other.bus_), type_(other.type_), id_(other.id_) {
            other.bus_ = nullptr;
        }

        Subscription& operator=(Subscription&& other) noexcept {
            if (this != &other) {
                unsubscribe();
                bus_ = other.bus_;
                type_ = other.type_;
                id_ = other.id_;
                other.bus_ = nullptr;
            }
            return *this;
        }

        ~Subscription() {
            unsubscribe();
        }

        void unsubscribe() {
            if (bus_ != nullptr) {
                bus_->unsubscribe(type_, id_);
                bus_ = nullptr;
            }
        }

    private:
        // TODO(learning): This is a non-owning raw pointer used by an RAII token.
        // If EventBus ever becomes heap-owned and shared between modules, replace
        // this with std::weak_ptr<EventBus> so subscriptions cannot outlive it.
        EventBus* bus_{nullptr};
        std::type_index type_{typeid(void)};
        std::size_t id_{0};
    };

    template <typename Event>
    Subscription subscribe(std::function<void(const Event&)> callback) {
        const auto type = std::type_index(typeid(Event));
        const auto id = nextId_++;

        auto wrapper = [callback = std::move(callback)](const void* event) {
            callback(*static_cast<const Event*>(event));
        };

        handlers_[type].push_back(Handler{id, std::move(wrapper)});
        return Subscription(*this, type, id);
    }

    template <typename Event>
    void publish(const Event& event) {
        const auto type = std::type_index(typeid(Event));
        const auto found = handlers_.find(type);
        if (found == handlers_.end()) {
            return;
        }

        const auto snapshot = found->second;
        for (const auto& handler : snapshot) {
            handler.callback(&event);
        }
    }

private:
    struct Handler {
        std::size_t id{};
        std::function<void(const void*)> callback;
    };

    void unsubscribe(std::type_index type, std::size_t id) {
        auto found = handlers_.find(type);
        if (found == handlers_.end()) {
            return;
        }

        auto& list = found->second;
        list.erase(
            std::remove_if(list.begin(), list.end(), [id](const Handler& handler) {
                return handler.id == id;
            }),
            list.end());
    }

    std::unordered_map<std::type_index, std::vector<Handler>> handlers_;
    std::size_t nextId_{1};
};

} // namespace Snake2
