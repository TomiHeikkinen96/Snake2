#include "Snake2/Plugins/SaveLoadPlugin.hpp"

#include "Snake2/Core/Events.hpp"
#include "Snake2/Game.hpp"

#include <fstream>
#include <iostream>
#include <utility>

namespace Snake2 {

HighScoreFile::HighScoreFile(std::filesystem::path path)
    : path_(std::move(path)) {}

int HighScoreFile::load() const {
    std::ifstream input(path_);
    int score = 0;
    input >> score;
    return score;
}

void HighScoreFile::save(int score) const {
    std::ofstream output(path_, std::ios::trunc);
    output << score << '\n';
}

SaveLoadPlugin::SaveLoadPlugin(std::filesystem::path path)
    : file_(std::move(path)) {}

void SaveLoadPlugin::onRegister(Game& game) {
    highScore_ = file_.load();
    std::cout << "High score: " << highScore_ << '\n';

    subscriptions_.push_back(game.events().subscribe<GameOverEvent>([this](const GameOverEvent& event) {
        if (event.score > highScore_) {
            highScore_ = event.score;
            file_.save(highScore_);
            std::cout << "New high score saved: " << highScore_ << '\n';
        }
    }));

    // TODO(learning): Save more state here, such as snake length or settings.
    // The file streams above are RAII objects: they close automatically when
    // leaving scope, even if an exception is thrown.
}

} // namespace Snake2
