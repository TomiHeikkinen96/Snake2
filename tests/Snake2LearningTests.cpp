#include "Snake2/Core/EventBus.hpp"
#include "Snake2/Core/Events.hpp"
#include "Snake2/ECS/Components.hpp"
#include "Snake2/ECS/EntityManager.hpp"
#include "Snake2/Plugins/SaveLoadPlugin.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace Snake2::Tests {
namespace {

class ScopedTempFile {
public:
    explicit ScopedTempFile(std::string name)
        : path_(std::filesystem::temp_directory_path() / std::move(name)) {
        std::filesystem::remove(path_);
    }

    ScopedTempFile(const ScopedTempFile&) = delete;
    ScopedTempFile& operator=(const ScopedTempFile&) = delete;

    ~ScopedTempFile() {
        std::error_code ignored;
        std::filesystem::remove(path_, ignored);
    }

    const std::filesystem::path& path() const {
        return path_;
    }

private:
    std::filesystem::path path_;
};

} // namespace

TEST(TestHarness, GoogleTestReportsPassAndFailInVisualStudio) {
    SUCCEED() << "If this appears in Test Explorer, GoogleTest and CTest discovery are wired correctly.";
}

TEST(EntityManagerCharacterization, EntitiesHaveStableIdsAndUniqueOwnedComponents) {
    EntityManager entities;

    auto& first = entities.createEntity();
    auto& second = entities.createEntity();
    first.addComponent<Position>(10, 20);
    second.addComponent<Velocity>(1, -1);

    EXPECT_NE(first.id(), second.id());
    ASSERT_NE(entities.get(first.id()), nullptr);
    ASSERT_NE(entities.get(second.id()), nullptr);
    EXPECT_EQ(entities.get(first.id())->getComponent<Position>()->x, 10);
    EXPECT_EQ(entities.get(second.id())->getComponent<Velocity>()->dy, -1);

    entities.destroyEntity(first.id());

    EXPECT_EQ(entities.get(first.id()), nullptr);
    EXPECT_NE(entities.get(second.id()), nullptr);
}

TEST(EventBusCharacterization, SubscriptionUnsubscribesWhenTokenLeavesScope) {
    EventBus bus;
    int observedScore = 0;

    {
        auto subscription = bus.subscribe<FoodEatenEvent>([&](const FoodEatenEvent& event) {
            observedScore = event.score;
        });
        bus.publish(FoodEatenEvent{7});
        EXPECT_EQ(observedScore, 7);
    }

    bus.publish(FoodEatenEvent{13});

    EXPECT_EQ(observedScore, 7);
}

TEST(EventBusCharacterization, MoveAssignmentReleasesPreviousSubscription) {
    EventBus bus;
    int firstHandlerCalls = 0;
    int secondHandlerCalls = 0;

    auto subscription = bus.subscribe<FoodEatenEvent>([&](const FoodEatenEvent&) {
        ++firstHandlerCalls;
    });

    subscription = bus.subscribe<FoodEatenEvent>([&](const FoodEatenEvent&) {
        ++secondHandlerCalls;
    });

    bus.publish(FoodEatenEvent{1});

    EXPECT_EQ(firstHandlerCalls, 0);
    EXPECT_EQ(secondHandlerCalls, 1);
}

TEST(HighScoreFileCharacterization, MissingFileLoadsAsZero) {
    ScopedTempFile file("snake2_missing_high_score_test.txt");

    const HighScoreFile highScore(file.path());

    EXPECT_EQ(highScore.load(), 0);
}

TEST(HighScoreFileCharacterization, SaveOverwritesPreviousScore) {
    ScopedTempFile file("snake2_saved_high_score_test.txt");
    const HighScoreFile highScore(file.path());

    highScore.save(4);
    highScore.save(9);

    EXPECT_EQ(highScore.load(), 9);
}

// The tests below are intentionally red. They keep the build green while giving
// you visible TDD tickets in Test Explorer. When you implement one feature,
// replace that FAIL() with production assertions instead of weakening the goal.

TEST(LearningTodo_EventBusLifetime, SubscriptionCanOutliveDestroyedSharedEventBusSafely) {
    FAIL() << "TODO(learning): redesign EventBus ownership so a subscription token cannot dereference a destroyed bus. "
              "Acceptance: destroying a heap-owned/shared EventBus before a remaining token is harmless and deterministic.";
}

TEST(LearningTodo_ComponentFactory, SnakeSegmentsAreCreatedThroughAFactoryWithGridCollider) {
    FAIL() << "TODO(learning): move snake segment construction into a factory and add a GridCollider-like component. "
              "Acceptance: head/body creation is testable without constructing Game or touching SDL rendering.";
}

TEST(LearningTodo_SystemStages, SystemsRunInNamedStagesWithDeterministicOrdering) {
    FAIL() << "TODO(learning): evolve SystemManager from a single vector into staged groups such as input, simulation, "
              "collision, and render. Acceptance: tests can register fake systems and prove stage order.";
}

TEST(LearningTodo_ScorePlugin, FastFoodStreakPublishesAchievementEvent) {
    FAIL() << "TODO(learning): add plugin behavior for a fast-eating streak and publish an achievement-style event. "
              "Acceptance: the plugin can be tested through EventBus events without relying on console output.";
}

TEST(LearningTodo_SaveLoadPlugin, SaveFileCanPersistHighScoreAndSettingsAtomically) {
    FAIL() << "TODO(learning): extend persistence beyond a single integer and make writes robust. "
              "Acceptance: high score plus settings round-trip through a temporary file, and malformed files fall back safely.";
}

TEST(LearningTodo_Concurrency, ReadOnlyGameSnapshotCanBeBuiltOffThread) {
    FAIL() << "TODO(learning): introduce a thread-safe read-only snapshot for future AI, replay, or networking work. "
              "Acceptance: a worker thread can consume a snapshot without sharing mutable ECS state.";
}

} // namespace Snake2::Tests
