#include "Snake2/Plugins/ScorePlugin.hpp"

#include "Snake2/Core/Events.hpp"
#include "Snake2/Game.hpp"

#include <iostream>

namespace Snake2 {

void ScorePlugin::onRegister(Game& game) {
    subscriptions_.push_back(game.events().subscribe<FoodEatenEvent>([this](const FoodEatenEvent& event) {
        score_ = event.score;
        std::cout << "Score: " << score_ << '\n';
    }));

    subscriptions_.push_back(game.events().subscribe<GameOverEvent>([this](const GameOverEvent& event) {
        std::cout << "Game over. Final score: " << event.score << '\n';
        score_ = 0;
    }));

    // TODO(learning): Add new plugin behavior here, such as awarding bonus
    // points for fast eating streaks or publishing an AchievementUnlockedEvent.
}

int ScorePlugin::score() const {
    return score_;
}

} // namespace Snake2
