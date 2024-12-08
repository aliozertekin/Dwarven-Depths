#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "Utils.h"
#include "Enemy.h"
#include "Game.h"
#include "Dwarf.h"
using namespace Game;

class Renderer {
public:

    static void handleInput(Dwarf& dwarf, Dwarf::Cursor& cursor, std::vector<std::vector<Tile>>& map, std::vector<Enemy>& enemies, std::map<std::string, int>& inventory);
	static std::string renderData(const std::map<std::string, int>& inventory);
	static std::string renderData(int y, const Dwarf& dwarf, const Dwarf::Cursor& cursor, int viewportStartY);
	static void render(
        const std::vector<std::vector<Tile>>& map,
        const Dwarf& dwarf,
        const Dwarf::Cursor& cursor,
        const std::vector<Enemy>& enemies,
        const std::map<std::string, int>& inventory);
	static void initializeMap(std::vector<std::vector<Tile>>& map);
private:

    static constexpr int CONSOLE_WIDTH = 64;  // Adjust based on your terminal size
    static constexpr int CONSOLE_HEIGHT = 24; // Exclude space for UI lines

};