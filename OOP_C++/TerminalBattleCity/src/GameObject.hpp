#pragma once

#include <vector>
#include <string>

class GameContext;

struct config {
    int max_speed = 5;
    int player_speed = 3;
    int enemy_speed = 2;
    int bullet_speed = 4;
    int min_speed = 1;
    int enemy_count = 3;
    int player_health = 3;
    int enemy_health = 1;

    config(int player_speed, int enemy_speed, int bullet_speed, int enemy_count):
    player_speed(player_speed), enemy_speed(enemy_speed), bullet_speed(bullet_speed), enemy_count(enemy_count) {}

    config() = default;
};

struct Position {
    int x = 0;
    int y = 0;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
};

enum Direct { UP, DOWN, RIGHT, LEFT };

class GameObject {
protected:
    char repr;
    Direct dir;
    Position pos;
    int health;
public:
    GameObject(Position pos, char repr, Direct dir, int health) : pos(pos), repr(repr), dir(dir), health(health) {}
    virtual ~GameObject() = default;

    virtual void act(GameContext& context) = 0;
    char render() const;

    bool isAlive() const;
    void hit();
    void setHealth(int health);

    Position getPos() const;
    Direct getDir() const;
    virtual void changePos(int dx, int dy);

    bool isWalkable(int x, int y, std::vector<std::string>& map) const;
};