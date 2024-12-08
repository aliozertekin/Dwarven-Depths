#pragma once
#include "Enemy.h"
#include "Game.h"
using namespace Game;

class Dwarf {
public:
    int x, y;
    int health;
    Direction direction;
    Dwarf(int startX, int startY, int startHealth = 50) : x(startX), y(startY), health(startHealth), direction(RIGHT) {}

    void repositionDwarf(std::vector<std::vector<Tile>>& map);
    void move(int dx, int dy, std::vector<std::vector<Tile>>& map);
    void takeDamage(int damage);
    void attack(std::vector<std::vector<Tile>>& map, std::vector<class Enemy>& enemies) const;
    void buyHp(std::map<std::string, int>& inventory, Dwarf& dwarf);

    static void checkDwarfEnemyCollision(Dwarf& dwarf, const std::vector<Enemy>& enemies, int frame);
    class Cursor {
    public:
        int x = NULL, y = NULL;
        static int floor;
        bool changeFloors = false;

        void updatePosition(const Dwarf& dwarf);
        void mine(std::vector<std::vector<Tile>>& map, std::map<std::string, int>& inventory);
    };
};