#include "Player.hpp"
#include "GameContext.hpp"

void Player::act(GameContext& context) {
    if (counter >= tact) {
        move(context);
        counter = 0;
    }
    ++counter;
}

void Player::move(GameContext& context) {
    int new_x = pos.x + pos_diff.x;
    int new_y = pos.y + pos_diff.y;
    pos_diff = {0, 0};
    if (isWalkable(new_x, new_y, context.map.getMap())) {
        pos = {new_x, new_y};
    }
}

void Player::changePos(int dx, int dy) {
    if (dx > 0) {
        dir = RIGHT;
        repr = '>';
    }
    else if (dx < 0) {
        dir = LEFT;
        repr = '<';
    }
    else if (dy > 0) {
        dir = DOWN;
        repr = 'v';
    }
    else if (dy < 0) {
        dir = UP;
        repr = '^';
    }
    pos_diff.x = dx;
    pos_diff.y = dy;
}
