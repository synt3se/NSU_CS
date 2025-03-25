#include <queue>
#include <algorithm>
#include <iostream>
#include <stack>

#include "GameContext.hpp"
#include "Enemy.hpp"

#include <memory>

bool Enemy::shouldShoot(GameContext &context) {
    return context.player->getPos().x == pos.x || context.player->getPos().y == pos.y;
}

void Enemy::createBullet(GameContext &context) {
    Position bullet_pos = pos;
    Direct bullet_dir = dir;
    switch (bullet_dir) {
        case UP:    bullet_pos.y -= 1; break;
        case DOWN:  bullet_pos.y += 1; break;
        case RIGHT: bullet_pos.x += 1; break;
        case LEFT:  bullet_pos.x -= 1; break;
    }
    context.addObject(std::make_shared<Bullet>(bullet_pos, bullet_dir, context.cfg));
    context.game_objects[context.game_objects.size() - 1]->act(context);
}

void Enemy::changeDir(Direct new_dir) {
    dir = new_dir;
    if (dir == RIGHT) {
        repr = '>';
    } else if (dir == LEFT) {
        repr = '<';
    } else if (dir == DOWN) {
        repr = 'v';
    } else if (dir == UP) {
        repr = '^';
    }
}

void Enemy::shoot(GameContext &context) {
    createBullet(context);
}

void Enemy::act(GameContext &context) {
    if (counter >= tact) {
        if (shouldShoot(context)) {
            shoot(context);
        }
        move(context);
        counter = 0;
    }
    ++counter;
}

void Enemy::move(GameContext &context) {
    if (nav.empty() || step > 6) { // calculate navigation every 6 steps
        nav = bfs(context);
        step = 0;
        if (nav.empty()) {
            return;
        }
    }
    ++step;

    changeDir(nav.top());
    nav.pop();

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
}

bool is_valid(int x, int y) {
    return x >= 0 && x < M && y >= 0 && y < N;
}

std::stack<Direct> Enemy::bfs(GameContext& context) {
    using namespace std;
    int startX = pos.x, startY = pos.y;
    int goalX = context.player->getPos().x, goalY = context.player->getPos().y;

    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};

    queue<pair<int, int>> q;
    q.push({startY, startX});

    vector<vector<int>> dist(N, vector<int>(M, -1));
    vector<vector<int>> parentX(N, vector<int>(M, -1));
    vector<vector<int>> parentY(N, vector<int>(M, -1));
    vector<vector<Direct>> direction(N, vector<Direct>(M, UP));

    dist[startY][startX] = 0;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (is_valid(nx, ny) && dist[ny][nx] == -1 && context.map.getMap()[ny][nx] != '#') {
                dist[ny][nx] = dist[y][x] + 1;
                parentX[ny][nx] = x;
                parentY[ny][nx] = y;
                switch (i) {
                    case 0: direction[ny][nx] = LEFT; break;
                    case 1: direction[ny][nx] = DOWN; break;
                    case 2: direction[ny][nx] = RIGHT; break;
                    case 3: direction[ny][nx] = UP; break;
                }
                q.emplace(ny, nx);

                if (ny == goalY && nx == goalX) {
                    q = {};
                    break;
                }
            }
        }
    }

    if (dist[goalY][goalX] == -1) {
        cout << "Path not found!" << endl;
        context.initGame(); // player or enemy stuck
        return {};
    }

    stack<Direct> path_stack;
    int currentY = goalY, currentX = goalX;

    while (currentY != startY || currentX != startX) {
        path_stack.push(direction[currentY][currentX]);
        int prevX = parentX[currentY][currentX];
        int prevY = parentY[currentY][currentX];
        currentX = prevX;
        currentY = prevY;
    }

    return path_stack;
}

void ModEnemy::shoot(GameContext &context) {
    Direct fix_dir = dir;
    Direct dirs[] = {UP, LEFT, DOWN, RIGHT};
    for (auto curr_dir: dirs) {
        dir = curr_dir;
        Enemy::shoot(context);
    }
    dir = fix_dir;
}
