#include "UI.hpp"
#include "GameContext.hpp"
#include "GameEngine.hpp"

namespace UI {
    using namespace ftxui;

    void ExecuteMainMenu(GameContext& context, std::function<void()> play,
        std::function<void()> quit, std::string& nickname) {
        ScreenInteractive screen = ScreenInteractive::Fullscreen();
        auto start_button = Button("Start", [&]() {
            play();
            screen.Exit();
        }, ButtonOption::Animated());
        auto quit_button = Button("Exit", [&]() {
            quit();
            screen.Exit();
        }, ButtonOption::Animated());

        Component input_nickname = Input(&nickname, "nickname");
        auto player_speed_slider = Slider("Player Speed", &context.cfg.player_speed, 0, 5, 1);
        auto enemy_speed_slider = Slider("Enemy Speed", &context.cfg.enemy_speed, 0, 5, 1);
        auto bullet_speed_slider = Slider("Bullet Speed", &context.cfg.bullet_speed, 0, 5, 1);
        auto enemy_count_slider = Slider("Enemy Count", &context.cfg.enemy_count, 1, 20, 1);

        auto container = Container::Vertical({
            start_button,
            quit_button,
            input_nickname,
            player_speed_slider,
            enemy_speed_slider,
            bullet_speed_slider,
            enemy_count_slider,
        });

        auto menu = Renderer(container, [&] {
            return vbox({
                text("Game Menu") | bold | hcenter,
                separator(),
                start_button->Render(),
                separator(),
                separator(),
                quit_button->Render(),
                separator(),
                hbox(text("Nickname : "), input_nickname->Render()),
                separator(),
                player_speed_slider->Render(),
                text(std::to_string(context.cfg.player_speed)) | size(WIDTH, EQUAL, 15),
                separator(),
                enemy_speed_slider->Render(),
                text(std::to_string(context.cfg.enemy_speed)) | size(WIDTH, EQUAL, 15),
                separator(),
                bullet_speed_slider->Render(),
                text(std::to_string(context.cfg.bullet_speed)) | size(WIDTH, EQUAL, 15),
                separator(),
                enemy_count_slider->Render(),
                text(std::to_string(context.cfg.enemy_count)) | size(WIDTH, EQUAL, 15),
            }) | flex | size(WIDTH, EQUAL, 50);
        });

        screen.Loop(menu);
    }

    Element Trophy() {
        return vbox({
            text(R"(_/\\\______________/\\\_/\\\\\\\\\\\_/\\\\\____/\\\\____/\\\____)"),
            text(R"(_\/\\\_____________\/\\\\/////\\\///_\/\\\\\\__\/\\\\___/\\\\\\\__)"),
            text(R"(__\/\\\_____________\/\\\__\/\\\______\/\\\/\\\__\/\\\__/\\\\\\\\\_)"),
            text(R"(___\//\\\____/\\\___/\\\____\/\\\______\/\\\//\\\_\/\\\_\//\\\\\\\__)"),
            text(R"(____\//\\\__/\\\\\__/\\\_____\/\\\______\/\\\\//\\\\/\\\__\//\\\\\___)"),
            text(R"(_____\\//\\\/\\\/\\/\\\_______\/\\\______\/\\\_\//\\\/\\\___\//\\\____)"),
            text(R"(______\//\\\\\\//\\\\\_________\/\\\______\/\\\__\//\\\\\\____\///_____)"),
            text(R"(_______\//\\\__\//\\\_______/\\\\\\\\\\\___\/\\\___\//\\\\\_____/\\\____)"),
            text(R"(________\///____\///________\///////////____\///_____\/////_____\///_____)"),
        });
    }

    void ExecuteWinScreen(int score) {
        ScreenInteractive screen = ScreenInteractive::Fullscreen();
        auto menu_button = Button("Menu", [&]() {
            screen.Exit();
        }, ButtonOption::Animated());
        auto win_text = Trophy();

        auto container = Container::Vertical({
            menu_button
        });
        auto menu = Renderer(container, [&] {
            return vbox({
                win_text | size(WIDTH, EQUAL, 80) | color(Color::Yellow),
                separator(),
                text("Score: " + std::to_string(score) + " points"),
                separator(),
                menu_button->Render() | size(WIDTH, EQUAL, 80)
            });
        });
        screen.Loop(menu);
    }

    Element Loss() {
        return vbox({
            text(R"(░▒▓█▓▒░      ░▒▓██████▓▒░ ░▒▓███████▓▒░▒▓████████▓▒░)"),
            text(R"(░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░)"),
            text(R"(░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░)"),
            text(R"(░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓██████▓▒░)"),
            text(R"(░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░)"),
            text(R"(░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░)"),
            text(R"(░▒▓████████▓▒░▒▓██████▓▒░░▒▓███████▓▒░░▒▓████████▓▒░)"),
        });
    };

    void ExecuteLoseScreen() {
        ScreenInteractive screen = ScreenInteractive::Fullscreen();
        auto menu_button = Button("Menu", [&]() {
            screen.Exit();
        }, ButtonOption::Animated());
        auto win_text = Loss();

        auto container = Container::Vertical({
            menu_button
        });
        auto menu = Renderer(container, [&] {
            return vbox({
                win_text | size(WIDTH, EQUAL, 80) | color(Color::Red),
                separator(),
                menu_button->Render(),
            });
        });
        screen.Loop(menu);
    }

    void ExecuteBoard(GameContext& context, Leaderboard& leaderboard, std::function<void()> on_win,
        std::function<void()> on_lose, int& score) {
        auto screen = ftxui::ScreenInteractive::Fullscreen();
        auto start = std::chrono::high_resolution_clock::now();
        context.initGame();
        for (int i = 0; i < context.cfg.enemy_count; ++i) {
            context.createEnemy();
        }
        Elements display_leaderboard = leaderboard.display();

        // Draw UI
        auto main_renderer = ftxui::Renderer([&] {
            std::vector<std::string> display_map = context.map.getMap();
            std::vector<std::string> display_obj = context.map.placeObjects(context.getGameObjects());
            int x = context.player->getPos().x;
            int y = context.player->getPos().y;
            Elements rows;
            std::string temp;
            for (int i = 0; i < N; ++i) {
                Elements row_elem;
                for (int j = 0; j < M; ++j) {
                    if (display_obj[i][j] != ' ') {
                        temp = display_obj[i][j];
                        row_elem.push_back(text(temp) | color(Color::Red) | bgcolor(Color::HSV(57, 97, 255)));
                    }
                    else if (i == y && j == x) {
                        temp = context.player->render();
                        row_elem.push_back(text(temp) | color(Color::Green) | bgcolor(Color::HSV(57, 97, 255)));
                    }
                    else {
                        temp = display_map[i][j];
                        row_elem.push_back(text(temp) | color(Color::Black) | bgcolor(Color::HSV(57, 97, 255)));
                    }
                }
                rows.push_back(hbox(std::move(row_elem)));
            }

            return vbox(rows, display_leaderboard);
        });

        // User input
        main_renderer |= ftxui::CatchEvent([&](ftxui::Event event) {
            if (event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('w')) {
                context.player->changePos(0, -1);
            } else if (event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('s')) {
                context.player->changePos(0, 1);
            } else if (event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('a')) {
                context.player->changePos(-1, 0);
            } else if (event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('d')) {
                context.player->changePos(1, 0);
            } else if (event == ftxui::Event::Character(' ')) {
                context.createBullet();
            }

            return false;
        });

        // Update content
        std::atomic<bool> refresh_ui_continue = true;
        std::thread refresh_ui([&] {
            while (refresh_ui_continue) {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(0.16s);
                if (context.game_objects.empty()) {
                    on_win();
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> duration = end - start;
                    config cfg = context.cfg;
                    score = (cfg.enemy_speed * cfg.enemy_count - cfg.player_speed) * 1000 / duration.count();
                    context.game_objects.clear();
                    screen.Exit();
                }
                if (!context.is_playing) {
                    on_lose();
                    context.game_objects.clear();
                    screen.Exit();
                }
                context.update();
                screen.PostEvent(ftxui::Event::Custom);
            }
        });

        screen.Loop(main_renderer);
        refresh_ui_continue = false;
        refresh_ui.join();
    }
}
