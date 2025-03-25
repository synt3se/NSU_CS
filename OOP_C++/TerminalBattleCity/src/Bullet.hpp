#pragma once

#include "GameObject.hpp"
#include "GameContext.hpp"

class Bullet : public GameObject {
public:
    int tact, counter = 0;

    Bullet(Position pos, Direct dir, config cfg) : GameObject(pos, '.', dir, 100),
    tact(cfg.max_speed - cfg.bullet_speed) {}

    void act(GameContext& context) override;
private:
    void move(GameContext& context);
};