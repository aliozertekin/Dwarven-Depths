#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <chrono>
#include <thread>
#include <string>
#include <thread>

#include "Utils.h"
#include "Game.h"
#include "Enemy.h"
#include "Dwarf.h"
#include "Renderer.h"

using namespace Game;



std::vector<Enemy> Enemy::enemies;
int Dwarf::Cursor::floor = 1;


int main() {
    clock_t delta_ticks;

    Utils::enableANSI();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::vector<Tile>> map(HEIGHT, std::vector<Tile>(EMPTY));
    Renderer::initializeMap(map);
    Dwarf dwarf(1, 1);

    Dwarf::Cursor cursor;
    cursor.updatePosition(dwarf);

    
    inventory["Gold"] = 0;
    inventory["Stone"] = 0;

    std::cout << "\033[2J\033[?25l";
    int frame = 0;

    while (true) {
        if (quitState) break; // Exit the game
        current_ticks = clock();
        Renderer::render(map, dwarf, cursor, Enemy::enemies, inventory);
        Renderer::handleInput(dwarf, cursor, map, Enemy::enemies, inventory);

        //Check for collisions
        Dwarf::checkDwarfEnemyCollision(dwarf, Enemy::enemies, frame);

        Enemy::moveEnemies(Enemy::enemies, map, frame);
        delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
        if (delta_ticks > 0) fps = CLOCKS_PER_SEC / delta_ticks;

        if (cursor.changeFloors) {
            std::cout << "\033[J";
            Utils::clearScreen();
            Renderer::initializeMap(map);
            dwarf.repositionDwarf(map);
            cursor.changeFloors = false;
        }

        cursor.updatePosition(dwarf);
        frame++;


    }

    std::cout << "\033[?25h" << "\nGame Over!";
    return 0;
}