#include "GameEngine.hpp"
#include "UI.hpp"

GameEngine::GameEngine(): leaderboard("leaderboard.txt") {
    state = GameState::Menu;
}

void GameEngine::run() {
    bool quit = false;
    auto on_quit = [&] { quit = true; };
    auto play = [&] { state = GameState::Playing; };
    auto on_win = [&] { state = GameState::Win; };
    auto on_lose = [&] { state = GameState::Lose; };
    while (!quit) {
        std::string nickname = "player";
        UI::ExecuteMainMenu(context, play, on_quit, nickname);
        if (quit) {
            break;
        }
        int score = 0;
        state = GameState::Playing;
        UI::ExecuteBoard(context, leaderboard, on_win, on_lose, score);
        if (state == GameState::Win) {
            UI::ExecuteWinScreen(score);
            leaderboard.addPlayer(nickname, score);
        }
        else if (state == GameState::Lose) {
            UI::ExecuteLoseScreen();
        }
    }
}
