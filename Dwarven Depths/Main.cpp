#include <thread>
#include <chrono>


#include "Utils.h"
#include "Game.h"
#include "Enemy.h"
#include "Dwarf.h"
#include "Renderer.h"

using namespace Game;



void gameLogic(std::vector<std::vector<Tile>>& map, Dwarf& dwarf, Dwarf::Cursor& cursor, int& frame, bool& quitState) {
    while (true) {
        if (quitState) break; // Exit the game
        updateTicks();
        // Check for collisions and move enemies
        {
            std::lock_guard<std::mutex> lockMap(mapMutex);
            std::lock_guard<std::mutex> lockEnemies(enemiesMutex);
            std::lock_guard<std::mutex> lockDwarf(dwarfMutex);

            Dwarf::checkDwarfEnemyCollision(dwarf, Enemy::enemies, frame);
            Enemy::moveEnemies(Enemy::enemies, map, frame);
        }

        // Handle floor changes
        if (cursor.changeFloors) {
            {
                std::lock_guard<std::mutex> lockMap(mapMutex);
                Renderer::initializeMap(map);
            }

            {
                std::lock_guard<std::mutex> lockDwarf(dwarfMutex);
                dwarf.repositionDwarf(map);
            }

            cursor.changeFloors = false;
        }

        // Update cursor position
        {
            std::lock_guard<std::mutex> lockCursor(cursorMutex);
            std::lock_guard<std::mutex> lockDwarf(dwarfMutex);
            cursor.updatePosition(dwarf);
        }

        frame++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Avoid high CPU usage
    }
}

void renderLoop(std::vector<std::vector<Tile>>& map, Dwarf& dwarf, Dwarf::Cursor& cursor, bool& quitState) {
    while (true) {
        if (quitState) break; // Exit the rendering thread

        // Render the game state
        {
            Renderer::render(map, dwarf, cursor, Enemy::enemies, inventory);
        }

        // Handle user input
        {
            Renderer::handleInput(dwarf, cursor, map, Enemy::enemies, inventory);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Maintain ~60 FPS
    }
}



int main() {
    Utils::enableANSI();
    Utils::hideCursorCleanScreen();
    startTimer();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::vector<Tile>> map(HEIGHT, std::vector<Tile>(EMPTY));
    Renderer::initializeMap(map);
    Dwarf dwarf(1, 1);

    Dwarf::Cursor cursor;   
    cursor.updatePosition(dwarf);

    inventory["Gold"] = 0;
    inventory["Stone"] = 0;

    int frame = 0;
    bool quitState = false; 

    // Create a thread for game logic
    std::thread gameLogicThread(gameLogic, std::ref(map), std::ref(dwarf), std::ref(cursor), std::ref(frame), std::ref(Game::quitState));

    // Create a thread for rendering
    std::thread renderThread(renderLoop, std::ref(map), std::ref(dwarf), std::ref(cursor), std::ref(Game::quitState));

    // Wait for both threads to finish (this will never be the case because `quitState` controls the exit)
    gameLogicThread.join();
    renderThread.join();

    std::cout << "\033[?25h" << "\nGame Over!";
    return 0;
}
