#pragma once

#include <SDL.h>

#include <memory>
#include <string>

namespace Snake2 {

class SDLContext {
public:
    SDLContext();
    ~SDLContext();

    SDLContext(const SDLContext&) = delete;
    SDLContext& operator=(const SDLContext&) = delete;
};

class Window {
public:
    Window(const std::string& title, int width, int height);

    SDL_Window* get() const {
        return window_.get();
    }

private:
    struct Deleter {
        void operator()(SDL_Window* window) const;
    };

    std::unique_ptr<SDL_Window, Deleter> window_;
};

class Renderer {
public:
    explicit Renderer(Window& window);

    SDL_Renderer* get() const {
        return renderer_.get();
    }

private:
    struct Deleter {
        void operator()(SDL_Renderer* renderer) const;
    };

    std::unique_ptr<SDL_Renderer, Deleter> renderer_;
};

} // namespace Snake2
