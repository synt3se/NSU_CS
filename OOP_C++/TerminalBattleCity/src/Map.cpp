#include "Map.hpp"
#include "GameObject.hpp"

Map::Map() {
    map = std::vector<std::string>(N, std::string(M, ' '));
}

void Map::initMap() {
    map = std::vector<std::string>(N, std::string(M, ' '));
    for (int i = 0; i < N; ++i) {
        map[i][0] = '#';
        map[i][M - 1] = '#';
    }
    for (int j = 0; j < M; ++j) {
        map[0][j] = '#';
        map[N - 1][j] = '#';
    }
    for (int i = 0; i < M + N; ++i) {
        int x = rand() % (M - 1) + 1, y = rand() % (N - 1) + 1;
        int length = rand() % 6 + 1, coord = rand() % 2;

        for (int j = 0; j < length; ++j) {
            if (coord == 0 && x + j < M) {
                map[y][x + j] = '#';
            }
            else if (coord == 1 && y + j < N) {
                map[y + j][x] = '#';
            }
        }
    }
}

std::vector<std::string> &Map::getMap() {
    return map;
}


std::vector<std::string> Map::placeObjects(std::vector<std::shared_ptr<GameObject>>& game_objects) {
    std::vector<std::string> display_map(N, std::string(M, ' '));;
    for (const std::shared_ptr<GameObject>& obj : game_objects) {
        int x = obj->getPos().x;
        int y = obj->getPos().y;
        display_map[y][x] = obj->render();
    }
    return display_map;
}
