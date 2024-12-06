#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <thread>

#ifndef KBHIT_H
#define KBHIT_H
#endif

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#endif

static int flagX, flagY;
static int score;
constexpr int MIN_WIDTH = 10;
constexpr int MAX_WIDTH = 100;
constexpr int MIN_HEIGHT = 15;
constexpr int MAX_HEIGHT = 100;
static clock_t current_ticks;
static clock_t fps = 0;
constexpr int HEALTH_COST = 5;
constexpr int HEALTH_BOUGHT = 100;
constexpr int FLAG_COST = 5;
static int WIDTH;
static int HEIGHT;
static bool quitState;
static bool flagState;

// Function to check for keypress
static bool cckbhit() {
#ifdef _WIN32
    return _kbhit();
#else
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);          // Save old terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  // Get old file status flags
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings
    fcntl(STDIN_FILENO, F_SETFL, oldf);      // Restore old file status flags

    if (ch != EOF) {
        ungetc(ch, stdin); // Put the character back into the input stream
        return true;
    }
    return false;
#endif
}

// Function to get a character
static char ccgetch() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);          // Save old terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings

    ch = getchar(); // Read a character

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings
    return ch;
#endif
}


static void enableANSI() {
#ifdef _WIN32
    // Enable ANSI escape codes for Windows
    std::system("clear"); // This sends an empty command to enable ANSI on Windows terminal
#else
    // On Unix/Linux, ANSI codes are supported by default
#endif
}

static void clearScreen() {
#ifdef _WIN32
    // Use ANSI codes to clear screen if enabled
    std::cout << "\033[2J\033[H";
#else
    // Use ANSI codes for Unix/Linux
    std::cout << "\033[2J\033[H";
#endif
    std::cout.flush(); // Ensure the commands are processed immediately
}


static void setMapSize() {
    WIDTH = MIN_WIDTH + (std::rand() % (MAX_WIDTH - MIN_WIDTH + 1));
    HEIGHT = MIN_HEIGHT + (std::rand() % (MAX_HEIGHT - MIN_HEIGHT + 1));
}

static std::vector<int> getMapSize() {
    return { HEIGHT,WIDTH };
}

enum TileWeight {
    WEIGHT_STONE = 20,
    WEIGHT_WALL = 30,
    WEIGHT_GOLD = 10
};
enum Tile {
    EMPTY = '.',
    WALL = '#',
    GOLD = 'G',
    STONE = 'S',
    DWARF = 'D',
    CURSOR = '+',
    BORDER = '@',
    ENEMY = 'E',
    FLAG = 'O'
};

enum class Color {
    RESET = 0,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

class Colorizer {
public:
    static std::string getColorCode(Color color) {
        switch (color) {
        case Color::RESET:   return "\033[0m";
        case Color::BLACK:   return "\033[30m";
        case Color::RED:     return "\033[31m";
        case Color::GREEN:   return "\033[32m";
        case Color::YELLOW:  return "\033[33m";
        case Color::BLUE:    return "\033[34m";
        case Color::MAGENTA: return "\033[35m";
        case Color::CYAN:    return "\033[36m";
        case Color::WHITE:   return "\033[37m";
        default: return "\033[0m";
        }
    }

    static std::string getTileColor(Tile tile) {
        switch (tile) {
        case DWARF:  return getColorCode(Color::MAGENTA);
        case CURSOR: return getColorCode(Color::CYAN);
        case WALL:   return getColorCode(Color::WHITE);
        case GOLD:   return getColorCode(Color::YELLOW);
        case STONE:  return getColorCode(Color::BLUE);
        case BORDER: return getColorCode(Color::RED);
        case ENEMY:  return getColorCode(Color::GREEN);
        case EMPTY:  return getColorCode(Color::BLACK);
        case FLAG: return getColorCode(Color::MAGENTA);
        default:     return getColorCode(Color::RESET);
        }
    }
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Dwarf {
public:
    int x, y;
    int health;
    Direction direction;

    Dwarf(int startX, int startY, int startHealth = 1000) : x(startX), y(startY), health(startHealth), direction(RIGHT) {}


    void move(int dx, int dy, std::vector<std::vector<Tile>>& map) {
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
    void takeDamage(int damage) {
        health -= damage;

        if (health <= 0) {
            health = 0;
            std::cout << "Game Over! The dwarf has fallen." << std::endl;
            quitState = true;
        }
    }

    // Attack in the direction the dwarf is facing
    void attack(std::vector<std::vector<Tile>>& map, std::vector<class Enemy>& enemies) const;
};

class Cursor {
public:

    int x = NULL, y = NULL;
    static int floor;
    bool changeFloors = false;

    void updatePosition(const Dwarf& dwarf) {
        switch (dwarf.direction) {
        case UP:    x = dwarf.x; y = dwarf.y - 1; break;
        case DOWN:  x = dwarf.x; y = dwarf.y + 1; break;
        case LEFT:  x = dwarf.x - 1; y = dwarf.y; break;
        case RIGHT: x = dwarf.x + 1; y = dwarf.y; break;
        }
        x = (std::max)(0, (std::min)(x, WIDTH - 1));
        y = (std::max)(0, (std::min)(y, HEIGHT - 1));
    }

    void mine(std::vector<std::vector<Tile>>& map, std::map<std::string, int>& inventory) {
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
            floor = floor++;
            score += 1000;
            changeFloors = true;
        }
    }
};
std::vector<std::pair<int, int>> emptyTiles;
static void initializeEmptyTiles(const std::vector<std::vector<Tile>>& map) {
    emptyTiles.clear();
    for (int y = 1; y < map.size() - 1; ++y) {
        for (int x = 1; x < map[0].size() - 1; ++x) {
            if (map[y][x] == EMPTY) emptyTiles.push_back({ x, y });
        }
    }
}
static std::pair<int, int> getRandomEmptyTile() {
    int index = std::rand() % emptyTiles.size();
    auto& tile = emptyTiles[index];
    emptyTiles.erase(emptyTiles.begin() + index);
    return tile;
}


static void repositionDwarf(Dwarf& dwarf, std::vector<std::vector<Tile>>& map) {
    if (dwarf.x < 0 || dwarf.x >= WIDTH || dwarf.y < 0 || dwarf.y >= HEIGHT || map[dwarf.y][dwarf.x] != EMPTY) {
        // Get a new random empty tile within the map
        std::pair<int, int> newPos = getRandomEmptyTile();
        dwarf.x = newPos.first;
        dwarf.y = newPos.second;
    }
}


class Enemy {
public:
    int x, y;
    int health;
    static std::vector<Enemy> enemies;
    static void addEnemy(int x, int y, int hp) {
        enemies.emplace_back(x, y, hp);
    }
    static std::vector<Enemy>& getEnemies() {
        return enemies;
    }
    Enemy(int startX, int startY, int hp = 1) : x(startX), y(startY), health(hp) {};

    void moveRandomly(std::vector<std::vector<Tile>>& map, int frame) {

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
    static void moveEnemies(std::vector<Enemy>& enemies, std::vector<std::vector<Tile>>& map, int frame) {
        if (frame % 5 != 0) return; // Process movement every 5th frame
        for (auto& enemy : enemies) {
            enemy.moveRandomly(map, frame);
        }
    }
};

static bool tileWeight(int weight) {
    if (std::rand() % 100 < weight) return true;
    else                            return false;
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
static void checkDwarfEnemyCollision(Dwarf& dwarf, const std::vector<Enemy>& enemies, int frame) {
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

static void repositionEnemies(std::vector<std::vector<Tile>>& map) {
    for (auto& enemy : Enemy::enemies) {
        if (enemy.x < 0 || enemy.x >= WIDTH || enemy.y < 0 || enemy.y >= HEIGHT || map[enemy.y][enemy.x] != EMPTY) {
            // Get a new random valid position within the map
            std::pair<int, int> newPos = getRandomEmptyTile();
            enemy.x = newPos.first;
            enemy.y = newPos.second;

        }
    }
}

static void initializeMap(std::vector<std::vector<Tile>>& map) {
    setMapSize();
    Enemy::enemies.clear(); // Clear all enemies before initializing the map
    map.clear();
    flagState = false;
    map.resize(HEIGHT, std::vector<Tile>(WIDTH, EMPTY));

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) {
                map[y][x] = BORDER;
            }
            else if (tileWeight(TileWeight::WEIGHT_WALL)) {
                map[y][x] = WALL;
            }
            else if (tileWeight(TileWeight::WEIGHT_GOLD)) {
                map[y][x] = GOLD;
            }
            else if (tileWeight(TileWeight::WEIGHT_STONE)) {
                map[y][x] = STONE;
            }
        }
    }
    initializeEmptyTiles(map);
    // Spawn enemies
    for (; Enemy::enemies.size() < std::floor((std::min)((std::min)(Cursor::floor / 2, 20), (WIDTH + HEIGHT) / 4));) {
        std::pair<int, int> enemyPosition = getRandomEmptyTile();
        map[enemyPosition.second][enemyPosition.first] = ENEMY;
        Enemy::addEnemy(enemyPosition.first, enemyPosition.second, 1);
    }

    // Place flag
    std::pair<int, int> newFlag = getRandomEmptyTile();
    flagY = newFlag.second;
    flagX = newFlag.first;
    map[newFlag.second][newFlag.first] = FLAG;

}
static void showFlag(std::map<std::string, int>& inventory) {
    if (inventory["Stone"] >= FLAG_COST && flagState == false) {
        inventory["Stone"] -= FLAG_COST;
        flagState = true;
    }
}
static std::string renderData(const std::map<std::string, int>& inventory) {
    std::string data;
    data += Colorizer::getColorCode(Color::CYAN) + "\n| Inventory:\n";
    const int ITEMS_PER_PAGE = 5;
    int currentPage = 0; // Track current page (can be toggled with input)
    auto it = inventory.begin();
    std::advance(it, currentPage * ITEMS_PER_PAGE);
    for (int i = 0; i < ITEMS_PER_PAGE && it != inventory.end(); ++i, ++it) {
        data += " - " + it->first + ": " + std::to_string(it->second) + "\n";
    }
    return data;

}
static std::string renderData(int y, const Dwarf& dwarf, const Cursor& cursor, int viewportStartY) {
    std::string data;
    int timeInSecond = ((current_ticks / 1000));
    int timeInMinute = (timeInSecond / 60);
    /*if (y == viewportStartY + 0) data += Colorizer::getColorCode(Color::WHITE) + "| Manual: 'WASD' to move.";
    if (y == viewportStartY + 1) data += Colorizer::getColorCode(Color::WHITE) + "| 'F' for attacking.";
    if (y == viewportStartY + 2) data += Colorizer::getColorCode(Color::WHITE) + "| 'M' to Mine tiles.";
    if (y == viewportStartY + 3) data += Colorizer::getColorCode(Color::WHITE) + "| 'B' to buy +"+std::to_string(HEALTH_BOUGHT)+"HP for -"+ std::to_string(HEALTH_COST)+"G.";
    if (y == viewportStartY + 4) data += Colorizer::getColorCode(Color::WHITE) + "| 'Q' to exit the game.";
    if (y == viewportStartY + 5) data += Colorizer::getColorCode(Color::WHITE) + "| Score: " + std::to_string(score);
    if (y == viewportStartY + 6) data += Colorizer::getColorCode(Color::WHITE) + "| Time: " + std::to_string(timeInMinute) + ":" + std::to_string(timeInSecond % 60);
    if (y == viewportStartY + 7) data += Colorizer::getColorCode(Color::YELLOW) + "| Dwarf Stats:";
    if (y == viewportStartY + 8) data += Colorizer::getColorCode(Color::RED)+"| Position: (" + std::to_string(dwarf.x) + ", " + std::to_string(dwarf.y) + ")";
    if (y == viewportStartY + 9) data += Colorizer::getColorCode(Color::RED) + "| Cursor: (" + std::to_string(cursor.x) + ", " + std::to_string(cursor.y) + ")";
    if (y == viewportStartY + 10) data += Colorizer::getColorCode(Color::RED) + "| Health: " + std::to_string(dwarf.health);
    if (y == viewportStartY + 11) data += Colorizer::getColorCode(Color::RED) + "| Floor: " + std::to_string(cursor.floor);
    if (y == viewportStartY + 12 && !flagState) data += Colorizer::getColorCode(Color::YELLOW) + "| 'E' to show the Flag location.(-"+std::to_string(FLAG_COST) + "S)";
    if (y == viewportStartY + 12 && flagState) data += Colorizer::getColorCode(Color::RED) + "| Flag Position: (" + std::to_string(flagX) + ", " + std::to_string(flagY) + ")";
    if (y == viewportStartY + 13) data += Colorizer::getColorCode(Color::GREEN) + "| Enemy amount: " + std::to_string(Enemy::enemies.size());*/
    std::vector<std::string> instructions = {
    Colorizer::getColorCode(Color::WHITE) + "| Manual: 'WASD' to move.",
    Colorizer::getColorCode(Color::WHITE) + "| 'F' for attacking.",
    Colorizer::getColorCode(Color::WHITE) + "| 'M' to Mine tiles.",
    Colorizer::getColorCode(Color::WHITE) + "| 'B' to buy +" + std::to_string(HEALTH_BOUGHT) + "HP for -" + std::to_string(HEALTH_COST) + "G.",
    Colorizer::getColorCode(Color::WHITE) + "| 'Q' to exit the game.",
    Colorizer::getColorCode(Color::YELLOW) + "| Stats:",
    Colorizer::getColorCode(Color::MAGENTA) + "| Score: " + std::to_string(score),
    Colorizer::getColorCode(Color::MAGENTA) + "| Time: [" + std::to_string(timeInMinute) + ":" + std::to_string(timeInSecond % 60) + "]",
    Colorizer::getColorCode(Color::RED) + "| Position: (" + std::to_string(dwarf.x) + ", " + std::to_string(dwarf.y) + ")",
    Colorizer::getColorCode(Color::RED) + "| Cursor: (" + std::to_string(cursor.x) + ", " + std::to_string(cursor.y) + ")",
    Colorizer::getColorCode(Color::RED) + "| Health: " + std::to_string(dwarf.health),
    Colorizer::getColorCode(Color::RED) + "| Floor: " + std::to_string(cursor.floor),
    Colorizer::getColorCode(Color::GREEN) + "| Enemy amount: " + std::to_string(Enemy::enemies.size())
    };
    if (!flagState) {
        instructions.push_back(Colorizer::getColorCode(Color::YELLOW) + "| 'E' to show the Flag location.(-" + std::to_string(FLAG_COST) + "S)");
    }
    else {
        instructions.push_back(Colorizer::getColorCode(Color::GREEN) + "| Flag Position: (" + std::to_string(flagX) + ", " + std::to_string(flagY) + ")");
    }
    if (y >= viewportStartY && y < viewportStartY + instructions.size()) {
        data += instructions[y - viewportStartY];
    }
    return data;
}
static void render(
    const std::vector<std::vector<Tile>>& map,
    const Dwarf& dwarf,
    const Cursor& cursor,
    const std::vector<Enemy>& enemies,
    const std::map<std::string, int>& inventory) {

    const int CONSOLE_WIDTH = 64;  // Adjust based on your terminal size
    const int CONSOLE_HEIGHT = 24; // Exclude space for UI lines

    const int VIEW_WIDTH = (std::min)(CONSOLE_WIDTH / 2, WIDTH);
    const int VIEW_HEIGHT = (std::min)(CONSOLE_HEIGHT - 8, HEIGHT); // Reserve space for UI

    // Determine viewport bounds
    int viewportStartX = (std::max)(0, (std::min)(dwarf.x - VIEW_WIDTH / 2, WIDTH - VIEW_WIDTH));
    int viewportStartY = (std::max)(0, (std::min)(dwarf.y - VIEW_HEIGHT / 2, HEIGHT - VIEW_HEIGHT));

    std::ostringstream buffer; // Off-screen buffer for double buffering

    // Clear the console (reset cursor position and clear output)
    buffer << "\033[H\033[J";

    // Render the viewport
    for (int y = viewportStartY; y < viewportStartY + VIEW_HEIGHT; ++y) {
        for (int x = viewportStartX; x < viewportStartX + VIEW_WIDTH; ++x) {
            char tileChar = static_cast<char>(map[y][x]);
            if (x == dwarf.x && y == dwarf.y) {
                tileChar = DWARF;
                buffer << Colorizer::getTileColor(Tile::DWARF);
            }
            else if (x == cursor.x && y == cursor.y) {
                tileChar = CURSOR;
                buffer << Colorizer::getTileColor(Tile::CURSOR);
            }
            else {
                buffer << Colorizer::getTileColor(map[y][x]);
            }

            buffer << tileChar << ' ';
        }
        buffer << renderData(y, dwarf, cursor, viewportStartY); // Render additional row data
        buffer << '\n'; // End of row
    }

    // Add the UI elements below the viewport
    buffer << renderData(inventory);

    // Reset color to default
    buffer << Colorizer::getColorCode(Color::RESET);
    buffer << "FPS:(MAX = 1000) = " + std::to_string(fps);
    // Flush the buffer to the console in one go
    std::cout << buffer.str();
}

static void handleInput(Dwarf& dwarf, Cursor& cursor, std::vector<std::vector<Tile>>& map, std::vector<Enemy>& enemies, std::map<std::string, int>& inventory) {
    if (cckbhit()) {
        char input = ccgetch();
        switch (input) {
        case 'w': dwarf.move(0, -1, map); break;
        case 's': dwarf.move(0, 1, map); break;
        case 'a': dwarf.move(-1, 0, map); break;
        case 'd': dwarf.move(1, 0, map); break;
        case 'm': cursor.mine(map, inventory); break;
        case 'f': dwarf.attack(map, enemies); break;
        case 'e': showFlag(inventory); break;
        case 'q': quitState = true; break;
        case 'b':
            if (inventory["Gold"] >= HEALTH_COST) {
                inventory["Gold"] -= HEALTH_COST;
                dwarf.health += HEALTH_BOUGHT;
            }
            break;
        default: break;
        }
    }
}
std::vector<Enemy> Enemy::enemies;
int Cursor::floor = 1;


int main() {
    clock_t delta_ticks;

    enableANSI();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::vector<Tile>> map(HEIGHT, std::vector<Tile>(EMPTY));
    initializeMap(map);
    Dwarf dwarf(1, 1);

    Cursor cursor;
    cursor.updatePosition(dwarf);

    std::map<std::string, int> inventory;
    inventory["Gold"] = 0;
    inventory["Stone"] = 0;

    std::cout << "\033[2J\033[?25l";
    int frame = 0;

    while (true) {
        if (quitState) break; // Exit the game
        current_ticks = clock();
        render(map, dwarf, cursor, Enemy::enemies, inventory);
        handleInput(dwarf, cursor, map, Enemy::enemies, inventory);

        //Check for collisions
        checkDwarfEnemyCollision(dwarf, Enemy::enemies, frame);

        Enemy::moveEnemies(Enemy::enemies, map, frame);
        delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
        if (delta_ticks > 0) fps = CLOCKS_PER_SEC / delta_ticks;

        if (cursor.changeFloors) {
            std::cout << "\033[J";
            clearScreen();
            std::cout << "New Floor: " << Cursor::floor << std::endl;
            initializeMap(map);
            repositionDwarf(dwarf, map);
            cursor.changeFloors = false;
        }

        cursor.updatePosition(dwarf);
        frame++;


    }

    std::cout << "\033[?25h" << "Game Over!" << std::endl;
    return 0;
}
