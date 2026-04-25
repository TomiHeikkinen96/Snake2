#pragma once

#include <SDL.h>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace Snake2 {

class ResourceManager {
public:
    explicit ResourceManager(SDL_Renderer* renderer);

    std::shared_ptr<SDL_Texture> loadTexture(const std::string& id, const std::filesystem::path& bmpPath);
    std::shared_ptr<SDL_Texture> texture(const std::string& id) const;

private:
    struct TextureGroup;

    struct TextureRef {
        std::shared_ptr<SDL_Texture> texture;

        // TODO(learning): This was deliberately designed as a possible
        // shared_ptr cycle: TextureGroup owns TextureRef objects, and TextureRef
        // wants to point back to its group. Keep this as weak_ptr. Changing it
        // to shared_ptr creates a cycle that leaks until you break it again.
        std::weak_ptr<TextureGroup> owner;
    };

    struct TextureGroup {
        std::unordered_map<std::string, std::shared_ptr<TextureRef>> textures;
    };

    // TODO(learning): This is a borrowed SDL handle. If the renderer lifetime
    // becomes less obvious, replace this raw pointer with an explicit owner or
    // a weak reference to a renderer service.
    SDL_Renderer* renderer_{nullptr};
    std::shared_ptr<TextureGroup> group_;
};

} // namespace Snake2
