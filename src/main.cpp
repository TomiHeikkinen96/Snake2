#define SDL_MAIN_HANDLED
#include "Snake2/Game.hpp"

#include <SDL.h>
#include <exception>
#include <iostream>

int main(int, char**) {
    SDL_SetMainReady();

    try {
        Snake2::Game game;
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "Snake2 failed: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
