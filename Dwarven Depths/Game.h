#pragma once

#include <iostream>
#include <vector>
#include <ctime> // For clock_t

namespace Game {
    static constexpr int MIN_WIDTH = 10;
    static constexpr int MAX_WIDTH = 100;

    static constexpr int MIN_HEIGHT = 15;
    static constexpr int MAX_HEIGHT = 100;
    static constexpr int HEALTH_COST = 5;
    static constexpr int HEALTH_BOUGHT = 100;
    static constexpr int FLAG_COST = 5;

    // Declare global variables with extern
    extern int flagX, flagY;
    extern int score;
    extern clock_t fps;
    extern clock_t current_ticks;
    extern int WIDTH, HEIGHT;
    extern bool quitState, flagState;

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
};
