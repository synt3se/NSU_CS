#include "Bullet.hpp"


void Bullet::act(GameContext& context) {
    if (counter >= tact) {
        move(context);
        counter = 0;
    }
    ++counter;
}

void Bullet::move(GameContext &context) {
    Position new_pos = pos;
    switch (dir) {
        case UP:    new_pos.y -= 1; break;
        case DOWN:  new_pos.y += 1; break;
        case RIGHT: new_pos.x += 1; break;
        case LEFT:  new_pos.x -= 1; break;
    }
    if (isWalkable(new_pos.x, new_pos.y, context.map.getMap()) && context.map.getMap()[pos.y][pos.x] == ' ') {
        pos = new_pos;
    }
    else {
        health = 0;
    }
}
