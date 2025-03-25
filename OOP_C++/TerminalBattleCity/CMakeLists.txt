cmake_minimum_required(VERSION 3.11)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

project(terminal_game LANGUAGES CXX VERSION 1.0.0)

add_executable(terminal_game
        src/game.cpp
        src/GameObject.hpp
        src/GameObject.cpp
        src/Player.hpp
        src/Player.cpp
        src/Enemy.hpp
        src/Enemy.cpp
        src/Bullet.hpp
        src/Bullet.cpp
        src/Map.hpp
        src/Map.cpp
        src/GameContext.hpp
        src/GameContext.cpp
        src/GameEngine.hpp
        src/GameEngine.cpp
        src/UI.cpp
        src/UI.hpp
        src/Leaderboard.hpp
        src/Leaderboard.cpp)


target_link_libraries(terminal_game
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)

target_include_directories(terminal_game PRIVATE
        C:/CPP_Projects/TerminalGame/FTXUI-main/include/ftxui
)