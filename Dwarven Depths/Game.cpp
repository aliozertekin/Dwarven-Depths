#include "Game.h"

namespace Game {
    // Define global variables here
    int flagX = 0, flagY = 0;
    int score = 0;
    clock_t fps = 0;
    clock_t current_ticks = 0;
    int WIDTH = MIN_WIDTH;
    int HEIGHT = MIN_HEIGHT;
    bool quitState = false;
    bool flagState = false;

    // Implement the Colorizer methods
    std::string Colorizer::getColorCode(Color color) {
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

    std::string Colorizer::getTileColor(Tile tile) {
        switch (tile) {
        case DWARF:  return getColorCode(Color::MAGENTA);
        case CURSOR: return getColorCode(Color::CYAN);
        case WALL:   return getColorCode(Color::WHITE);
        case GOLD:   return getColorCode(Color::YELLOW);
        case STONE:  return getColorCode(Color::BLUE);
        case BORDER: return getColorCode(Color::RED);
        case ENEMY:  return getColorCode(Color::GREEN);
        case EMPTY:  return getColorCode(Color::BLACK);
        case FLAG:   return getColorCode(Color::MAGENTA);
        default:     return getColorCode(Color::RESET);
        }
    }

    // Implement setMapSize and getMapSize
    void setMapSize() {
        WIDTH = MIN_WIDTH + (rand() % (MAX_WIDTH - MIN_WIDTH + 1));
        HEIGHT = MIN_HEIGHT + (rand() % (MAX_HEIGHT - MIN_HEIGHT + 1));
        std::cout << "Map size set to " << WIDTH << "x" << HEIGHT << std::endl;
    }

    std::vector<int> getMapSize() {
        return { WIDTH, HEIGHT };
    }
}
