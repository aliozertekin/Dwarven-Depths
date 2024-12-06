#include "Enemy.h"
#include <cstdlib>

// Define the static member
std::vector<Enemy> Enemy::enemies;

void Enemy::addEnemy(int x, int y, int hp) {
    enemies.emplace_back(x, y, hp);
}

std::vector<Enemy>& Enemy::getEnemies() {
    return enemies;
}

void Enemy::moveRandomly(std::vector<std::vector<Game::Tile>>& map, Enemy& enemy) {
    int dx = 0, dy = 0;
    switch (std::rand() % 4) {
    case 0: dy = -1; break; // Move up
    case 1: dy = 1;  break; // Move down
    case 2: dx = -1; break; // Move left
    case 3: dx = 1;  break; // Move right
    }

    int newX = enemy.x + dx;
    int newY = enemy.y + dy;

    // Check if the new position is valid
    if (newX >= 1 && newX < Game::WIDTH - 1 && newY >= 1 && newY < Game::HEIGHT - 1 &&
        map[newY][newX] == Game::Tile::EMPTY) {
        // Clear old position and set new position
        map[enemy.y][enemy.x] = Game::Tile::EMPTY;
        enemy.x = newX;
        enemy.y = newY;
        map[enemy.y][enemy.x] = Game::Tile::ENEMY;
    }
}

void Enemy::moveEnemies(std::vector<std::vector<Game::Tile>>& map, int frame) {
    if (frame % 5 != 0) return; // Process movement every 5th frame

    for (auto& enemy : enemies) {
        moveRandomly(map, enemy);
    }
}

void Enemy::repositionEnemies(std::vector<std::vector<Game::Tile>>& map) {
    for (auto& enemy : enemies) {
        if (enemy.x < 0 || enemy.x >= Game::WIDTH || enemy.y < 0 || enemy.y >= Game::HEIGHT ||
            map[enemy.y][enemy.x] != Game::Tile::EMPTY) {
            // Get a new random valid position within the map
            std::pair<int, int> newPos = getRandomEmptyTile(map);
            enemy.x = newPos.first;
            enemy.y = newPos.second;
        }
    }
}

std::pair<int, int> Enemy::getRandomEmptyTile(const std::vector<std::vector<Game::Tile>>& map) {
    int x, y;
    do {
        x = std::rand() % Game::WIDTH;
        y = std::rand() % Game::HEIGHT;
    } while (map[y][x] != Game::Tile::EMPTY);
    return { x, y };
}
