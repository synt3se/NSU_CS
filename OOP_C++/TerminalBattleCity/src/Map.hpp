#pragma once

#include <memory>
#include <vector>

#define N 20
#define M 40

class GameObject;

class Map {
public:
    Map();
    void initMap();

    std::vector<std::string> &getMap();
    std::vector<std::string> placeObjects(std::vector<std::shared_ptr<GameObject>>& game_objects);
private:
    std::vector<std::string> map;
};

