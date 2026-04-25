#pragma once

#include "Snake2/Core/EventBus.hpp"
#include "Snake2/Plugins/Plugin.hpp"

#include <filesystem>
#include <vector>

namespace Snake2 {

class HighScoreFile {
public:
    explicit HighScoreFile(std::filesystem::path path);

    int load() const;
    void save(int score) const;

private:
    std::filesystem::path path_;
};

class SaveLoadPlugin final : public Plugin {
public:
    explicit SaveLoadPlugin(std::filesystem::path path);
    void onRegister(Game& game) override;

private:
    HighScoreFile file_;
    int highScore_{0};
    std::vector<EventBus::Subscription> subscriptions_;
};

} // namespace Snake2
