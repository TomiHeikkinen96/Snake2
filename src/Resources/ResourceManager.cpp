#include "Snake2/Resources/ResourceManager.hpp"

#include <stdexcept>

namespace Snake2 {

ResourceManager::ResourceManager(SDL_Renderer* renderer)
    : renderer_(renderer), group_(std::make_shared<TextureGroup>()) {}

std::shared_ptr<SDL_Texture> ResourceManager::loadTexture(const std::string& id, const std::filesystem::path& bmpPath) {
    auto* surface = SDL_LoadBMP(bmpPath.string().c_str());
    if (surface == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    auto surfaceGuard = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(surface, SDL_FreeSurface);
    auto* rawTexture = SDL_CreateTextureFromSurface(renderer_, surfaceGuard.get());
    if (rawTexture == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    auto texture = std::shared_ptr<SDL_Texture>(rawTexture, SDL_DestroyTexture);
    auto ref = std::make_shared<TextureRef>();
    ref->texture = texture;
    ref->owner = group_;
    group_->textures[id] = std::move(ref);
    return texture;
}

std::shared_ptr<SDL_Texture> ResourceManager::texture(const std::string& id) const {
    const auto found = group_->textures.find(id);
    if (found == group_->textures.end()) {
        return {};
    }
    return found->second->texture;
}

} // namespace Snake2
