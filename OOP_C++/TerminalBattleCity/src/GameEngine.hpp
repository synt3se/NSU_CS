#pragma once

#include <atomic>
#include <thread>
#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "GameContext.hpp"
#include "GameObject.hpp"
#include "Leaderboard.hpp"
#include "UI.hpp"

enum class GameState { Menu, Playing, Win, Lose };

using namespace ftxui;

class GameEngine {
public:
    GameContext context;
    GameState state;
    Leaderboard leaderboard;

    GameEngine();
    void run();
};

