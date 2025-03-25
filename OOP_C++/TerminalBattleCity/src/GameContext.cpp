#include "GameContext.hpp"

void GameContext::initGame() {
    map.initMap();
    is_playing = true;
    Position player_pos = {0, 0};
    while (map.getMap()[player_pos.y][player_pos.x] != ' ') {
        int x = rand() % (M - 2) + 1;
        int y = rand() % (N - 2) + 1;
        player_pos = {x, y};
    }
    if (player) {
        player->setHealth(cfg.player_health);
    }
    else {
        player = std::make_shared<Player>(player_pos, cfg);
    }
}

void GameContext::addObject(std::shared_ptr<GameObject> obj) {
    game_objects.emplace_back(std::move(obj));
}

std::shared_ptr<Player> GameContext::getPlayer() {
    return player;
}

std::vector<std::shared_ptr<GameObject>>& GameContext::getGameObjects() {
    return game_objects;
}

void GameContext::createBullet() {
    Position bullet_pos = player->getPos();
    Direct bullet_dir = player->getDir();
    switch (bullet_dir) {
        case UP:    bullet_pos.y -= 1; break;
        case DOWN:  bullet_pos.y += 1; break;
        case RIGHT: bullet_pos.x += 1; break;
        case LEFT:  bullet_pos.x -= 1; break;
    }
    addObject(std::make_shared<Bullet>(bullet_pos, bullet_dir, cfg));
    game_objects[game_objects.size() - 1]->act(*this);
}

void GameContext::createEnemy() {
    Position pos = {0, 0};
    Position p_pos = player->getPos();
    while (map.getMap()[pos.y][pos.x] != ' ' || abs(p_pos.x - pos.x) + abs(p_pos.y - pos.y) < 8) {
        int x = rand() % (M - 1) + 1;
        int y = rand() % (N - 1) + 1;
        pos = {x, y};
    }
    int enemy_type = rand() % 2;
    if (enemy_type) { // default enemy
        addObject(std::make_shared<Enemy>(pos, cfg));
    }
    else { // modern enemy
        addObject(std::make_shared<ModEnemy>(pos, cfg));
    }

}

void GameContext::checkCollisions() {
    for (int i = 0; i < game_objects.size(); ++i) {
        if (game_objects[i]->getPos().x == player->getPos().x &&
            game_objects[i]->getPos().y == player->getPos().y) {
            player->hit();
            game_objects[i]->hit();
        }
        for (int j = i + 1; j < game_objects.size(); ++j) {
            if (!game_objects[i]->isAlive() || !game_objects[j]->isAlive()) {
                continue;
            }
            Position first = game_objects[i]->getPos();
            Position second = game_objects[j]->getPos();
            if (first.x == second.x && first.y == second.y) {
                game_objects[i]->hit();
                game_objects[j]->hit();
            }
        }
    }
}

void GameContext::update() {
    if (player->isAlive()) {
        player->act(*this);
    }
    else {
        is_playing = false;
        return;
    }
    for (int i = 0; i < game_objects.size(); ) {
        if (game_objects[i]->isAlive()) {
            game_objects[i]->act(*this);
            ++i;
        }
        else {
            game_objects.erase(game_objects.begin() + i);
        }
    }
    checkCollisions();
}
