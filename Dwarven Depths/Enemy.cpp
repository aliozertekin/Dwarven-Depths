#include "Enemy.h"

void Enemy::addEnemy(int x, int y, int hp)
{
    enemies.emplace_back(x, y, hp);
}

std::vector<Enemy>& Enemy::getEnemies()
{
    return enemies;
}

void Enemy::repositionEnemies(std::vector<std::vector<Tile>>& map) 
{
    for (auto& enemy : Enemy::enemies) {
        if (enemy.x < 0 || enemy.x >= WIDTH || enemy.y < 0 || enemy.y >= HEIGHT || map[enemy.y][enemy.x] != EMPTY) {
            // Get a new random valid position within the map
            std::pair<int, int> newPos = getRandomEmptyTile();
            enemy.x = newPos.first;
            enemy.y = newPos.second;

        }
    }
}

void Enemy::moveRandomly(std::vector<std::vector<Tile>>& map, int frame)
{

    int dx = 0, dy = 0;
    switch (std::rand() % 4) {
    case 0: dy = -1; break; // Move up
    case 1: dy = 1;  break; // Move down
    case 2: dx = -1; break; // Move left
    case 3: dx = 1;  break; // Move right
    }

    int newX = x + dx;
    int newY = y + dy;

    // Check if the new position is valid
    if (newX >= 1 && newX < WIDTH - 1 && newY >= 1 && newY < HEIGHT - 1 && map[newY][newX] == EMPTY) {
        map[y][x] = EMPTY;
        // Update map: clear old position and set new position
        x = newX;
        y = newY;

        map[y][x] = ENEMY;
    }

}

void Enemy::moveEnemies(std::vector<Enemy>& enemies, std::vector<std::vector<Tile>>& map, int frame)
{
    if (frame % 5 != 0) return; // Process movement every 5th frame
    for (auto& enemy : enemies) {
        enemy.moveRandomly(map, frame);
    }
}
