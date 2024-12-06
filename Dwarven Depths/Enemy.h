#pragma once
#include <vector>
#include "Game.h"

using namespace Game;
class Enemy {
public:
    int x, y;
    int health;
    static std::vector<Enemy> enemies;
    static void moveEnemies(std::vector<Enemy>& enemies, std::vector<std::vector<Tile>>& map, int frame);
    static void addEnemy(int x, int y, int hp);
    static std::vector<Enemy>& getEnemies();
    static void repositionEnemies(std::vector<std::vector<Tile>>& map);
    void moveRandomly(std::vector<std::vector<Tile>>& map, int frame);
    Enemy(int startX, int startY, int hp = 1) : x(startX), y(startY), health(hp) {};
};