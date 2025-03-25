#include "GameObject.hpp"
#include "GameContext.hpp"

char GameObject::render() const {
    return repr;
}

void GameObject::hit() {
    --health;
}

void GameObject::setHealth(int hp) {
    health = hp;
}


void GameObject::changePos(int dx, int dy) {
    pos.x += dx;
    pos.y += dy;
}

bool GameObject::isAlive() const {
    return health > 0;
}

Direct GameObject::getDir() const {
    return dir;
}

Position GameObject::getPos() const {
    return pos;
}

bool GameObject::isWalkable(int x, int y, std::vector<std::string> &map) const {
    if (x > 0 && x < map[0].size() && y > 0 && y < map.size())
        return map[y][x] == ' ';
    return false;
}
