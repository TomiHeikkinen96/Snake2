#pragma once

namespace Snake2 {

class Game;

class Plugin {
public:
    virtual ~Plugin() = default;
    virtual void onRegister(Game& game) = 0;
};

} // namespace Snake2
