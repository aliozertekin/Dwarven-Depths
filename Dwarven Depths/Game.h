#pragma once
#include <mutex>
#include <iostream>
#include <vector>
#include <ctime> // For clock_t
#include <map>

namespace Game {
    static constexpr int MIN_WIDTH = 10;
    static constexpr int MAX_WIDTH = 100;
    static constexpr int MIN_HEIGHT = 15;
    static constexpr int MAX_HEIGHT = 100;
    static constexpr int HEALTH_COST = 5;
    static constexpr int HEALTH_BOUGHT = 10;
    static constexpr int FLAG_COST = 5;
    // Mutex for locking 
    extern std::mutex mapMutex;
    extern std::mutex enemiesMutex;
    extern std::mutex dwarfMutex;
    extern std::mutex cursorMutex;
    extern std::mutex inventoryMutex;
    // Declare global variables with extern
    extern int WIDTH, HEIGHT;
    extern int flagX, flagY;
    extern int score;
    extern clock_t current_ticks;
    extern bool quitState, flagState;
    extern std::vector<std::pair<int, int>> emptyTiles;
    extern std::map<std::string, int> inventory;

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
        static std::string getColorCode(Color color);
        static std::string getTileColor(Tile tile);
    };

    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    // Function declarations
    void setMapSize();
    std::vector<int> getMapSize();
    void initializeEmptyTiles(const std::vector<std::vector<Tile>>& map);
    std::pair<int, int> getRandomEmptyTile();
    bool tileWeight(int weight);
    void showFlag(std::map<std::string, int>& inventory);
    std::vector<int> getTime();
    void updateTicks();
    void startTimer();

};
