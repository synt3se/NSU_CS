#pragma once

#include <stack>
#include "GameObject.hpp"

class Enemy : public GameObject { // TODO: Different type of enemy
public:
    int tact, counter = 0;

    Enemy(Position pos, config cfg) : GameObject(pos, '^', UP, cfg.enemy_health),
    tact(cfg.max_speed - cfg.enemy_speed) {}

    void act(GameContext& context) override;
protected:
    virtual void shoot(GameContext& context);
private:
    std::stack<Direct> nav;
    int step = 0;

    void move(GameContext& context);

    void changeDir(Direct new_dir);

    bool shouldShoot(GameContext& context);

    void createBullet(GameContext& context);

    std::stack<Direct> bfs(GameContext& context);
};

class ModEnemy: public Enemy {
public:
    ModEnemy(Position pos, config cfg) : Enemy(pos, cfg) {}
private:
    void shoot(GameContext &context) override;
};

