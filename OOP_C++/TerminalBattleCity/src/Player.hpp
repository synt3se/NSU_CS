#pragma once

#include "GameObject.hpp"

class Player : public GameObject {
private:
    Position pos_diff = {0, 0};

    void move(GameContext& context);
public:
    int tact, counter = 0;

    Player(Position pos, config cfg) : GameObject(pos, '^', UP, cfg.player_health),
    tact(cfg.max_speed - cfg.player_speed) {}

    void act(GameContext& context) override;
    void changePos(int dx, int dy) override;
};