#pragma once

#include "Snake2/Core/EventBus.hpp"
#include "Snake2/Plugins/Plugin.hpp"

#include <vector>

namespace Snake2 {

class ScorePlugin final : public Plugin {
public:
    void onRegister(Game& game) override;
    int score() const;

private:
    int score_{0};
    std::vector<EventBus::Subscription> subscriptions_;
};

} // namespace Snake2
