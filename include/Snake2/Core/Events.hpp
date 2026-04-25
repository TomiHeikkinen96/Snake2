#pragma once

#include <cstdint>

namespace Snake2 {

using EntityId = std::uint32_t;

struct QuitEvent {};

struct FoodEatenEvent {
    int score{};
};

struct GameOverEvent {
    int score{};
};

} // namespace Snake2
