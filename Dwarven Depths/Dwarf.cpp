#include "Dwarf.h"



int Dwarf::Cursor::floor = 1;

void Dwarf::repositionDwarf(std::vector<std::vector<Tile>>& map)
{
    if (Dwarf::x < 0 || Dwarf::x >= WIDTH || Dwarf::y < 0 || Dwarf::y >= HEIGHT || map[Dwarf::y][Dwarf::x] != EMPTY) {
        // Get a new random empty tile within the map
        std::pair<int, int> newPos = getRandomEmptyTile();
        Dwarf::x = newPos.first;
        Dwarf::y = newPos.second;
    }
}
void Dwarf::move(int dx, int dy, std::vector<std::vector<Tile>>& map)
{
    std::lock_guard<std::mutex> lockMap(mapMutex);
    std::lock_guard<std::mutex> lockDwarf(dwarfMutex);


    int newX = x + dx;
    int newY = y + dy;

    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT && map[newY][newX] == EMPTY) {
        x = newX;
        y = newY;
    }

    if (dx == 1) direction = RIGHT;
    else if (dx == -1) direction = LEFT;
    else if (dy == 1) direction = DOWN;
    else if (dy == -1) direction = UP;
}

void Dwarf::takeDamage(int damage)
{
    health -= damage;

    if (health <= 0) {
        health = 0;
        std::cout << "Game Over! The dwarf has fallen." << std::endl;
        quitState = true;
    }
}

void Dwarf::checkDwarfEnemyCollision(Dwarf& dwarf, const std::vector<Enemy>& enemies, int frame)
{
    if (frame % 2 != 0) return; // Collision check every other frame

    // Create a temporary map to store enemy positions for this frame
    std::map<std::pair<int, int>, int> currentEnemyPositions;

    // Populate the map with the current positions of all enemies
    for (const auto& enemy : enemies) {
        currentEnemyPositions[{enemy.x, enemy.y}] = 1;
    }

    // Check if the dwarf's position matches any enemy position
    if (currentEnemyPositions.find({ dwarf.x, dwarf.y }) != currentEnemyPositions.end()) {
        dwarf.takeDamage(Cursor::floor);
        std::cout << "The dwarf collided with an enemy! Health: " << dwarf.health << std::endl;
    }
}

void Dwarf::attack(std::vector<std::vector<Tile>>& map, std::vector<Enemy>& enemies) const {
    int targetX = x, targetY = y;

    switch (direction) {
    case UP:    targetY -= 1; break;
    case DOWN:  targetY += 1; break;
    case LEFT:  targetX -= 1; break;
    case RIGHT: targetX += 1; break;
    }

    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (it->x == targetX && it->y == targetY) {
            it->health -= 1; // Reduce enemy health
            if (it->health <= 0) {
                map[targetY][targetX] = EMPTY; // Remove enemy from map
                score += 500;
                enemies.erase(it);            // Remove enemy from list
            }
            return; // Exit loop after processing ( this was break before. )
        }
    }
}

void Dwarf::buyHp(std::map<std::string, int>& inventory, Dwarf& dwarf)
{
    if (inventory["Gold"] >= HEALTH_COST) {
        inventory["Gold"] -= HEALTH_COST;
        dwarf.health += HEALTH_BOUGHT;
    }
}

void Dwarf::Cursor::updatePosition(const Dwarf& dwarf)
{
    switch (dwarf.direction) {
    case UP:    x = dwarf.x; y = dwarf.y - 1; break;
    case DOWN:  x = dwarf.x; y = dwarf.y + 1; break;
    case LEFT:  x = dwarf.x - 1; y = dwarf.y; break;
    case RIGHT: x = dwarf.x + 1; y = dwarf.y; break;
    }
    x = (std::max)(0, (std::min)(x, WIDTH - 1));
    y = (std::max)(0, (std::min)(y, HEIGHT - 1));
}

void Dwarf::Cursor::mine(std::vector<std::vector<Tile>>& map, std::map<std::string, int>& inventory)
{
    if (map[y][x] == GOLD) {
        inventory["Gold"]++;
        score += 100;
        map[y][x] = EMPTY;
    }
    else if (map[y][x] == STONE) {
        inventory["Stone"]++;
        score += 50;
        map[y][x] = EMPTY;
    }
    else if (map[y][x] == WALL) {
        score += 1;
        map[y][x] = EMPTY;
    }
    else if (map[y][x] == FLAG) {
        
        floor++;
        score += 1000;
        changeFloors = true;
    }
};
