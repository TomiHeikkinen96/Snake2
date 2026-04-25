#pragma once

#include <SDL.h>

namespace Snake2 {

struct IComponent {
    virtual ~IComponent() = default;
};

struct Position final : IComponent {
    Position() = default;
    Position(int xValue, int yValue) : x(xValue), y(yValue) {}

    int x{};
    int y{};
};

struct Velocity final : IComponent {
    Velocity() = default;
    Velocity(int dxValue, int dyValue) : dx(dxValue), dy(dyValue) {}

    int dx{};
    int dy{};
};

struct Renderable final : IComponent {
    Renderable() = default;
    explicit Renderable(SDL_Color renderColor) : color(renderColor) {}

    SDL_Color color{};
};

struct SnakeSegment final : IComponent {
    SnakeSegment() = default;
    explicit SnakeSegment(bool head) : isHead(head) {}

    bool isHead{};
};

struct Food final : IComponent {};

} // namespace Snake2
