#pragma once

#include <atomic>
#include <thread>
#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "GameContext.hpp"
#include "Leaderboard.hpp"

namespace UI {
    void ExecuteMainMenu(GameContext& context, std::function<void()> play,
        std::function<void()> quit, std::string& nickname);
    void ExecuteWinScreen(int score);
    void ExecuteLoseScreen();
    void ExecuteBoard(GameContext& context, Leaderboard& leaderboard, std::function<void()> on_win,
        std::function<void()> on_lose, int& score);
}

