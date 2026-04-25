#include "Snake2/Core/SDLWrappers.hpp"

#include <stdexcept>

namespace Snake2 {

SDLContext::SDLContext() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(SDL_GetError());
    }
}

SDLContext::~SDLContext() {
    SDL_Quit();
}

Window::Window(const std::string& title, int width, int height)
    : window_(SDL_CreateWindow(
          title.c_str(),
          SDL_WINDOWPOS_CENTERED,
          SDL_WINDOWPOS_CENTERED,
          width,
          height,
          SDL_WINDOW_SHOWN)) {
    if (!window_) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Window::Deleter::operator()(SDL_Window* window) const {
    SDL_DestroyWindow(window);
}

Renderer::Renderer(Window& window)
    : renderer_(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
    if (!renderer_) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::Deleter::operator()(SDL_Renderer* renderer) const {
    SDL_DestroyRenderer(renderer);
}

} // namespace Snake2
