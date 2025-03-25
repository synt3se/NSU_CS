#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Map.hpp"
#include "GameObject.hpp"
#include "Bullet.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

class GameContext {
public:
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<GameObject>> game_objects;
    bool is_playing = true;
    config cfg;
    Map map;

    GameContext() : cfg(config()) {
        initGame();
    }

    void initGame();
    void addObject(std::shared_ptr<GameObject> obj);
    void createBullet();
    void createEnemy();

    std::shared_ptr<Player> getPlayer();
    std::vector<std::shared_ptr<GameObject>>& getGameObjects();

    void update();
    void checkCollisions();
};

