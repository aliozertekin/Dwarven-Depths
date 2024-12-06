#include "Renderer.h"



void Renderer::handleInput(Dwarf& dwarf, Dwarf::Cursor& cursor, std::vector<std::vector<Tile>>& map, std::vector<Enemy>& enemies, std::map<std::string, int>& inventory)
{
    if (Utils::cckbhit()) {
        char input = Utils::ccgetch();
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

std::string Renderer::renderData(const std::map<std::string, int>& inventory)
{
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
std::string Renderer::renderData(int y, const Dwarf& dwarf, const Dwarf::Cursor& cursor, int viewportStartY)
{
    std::string data;
    int timeInSecond = ((current_ticks / 1000));
    int timeInMinute = (timeInSecond / 60);
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

void Renderer::render(
    const std::vector<std::vector<Tile>>& map,
    const Dwarf& dwarf,
    const Dwarf::Cursor& cursor,
    const std::vector<Enemy>& enemies,
    const std::map<std::string, int>& inventory)
{
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
        buffer << Renderer::renderData(y, dwarf, cursor, viewportStartY); // Render additional row data
        buffer << '\n'; // End of row
    }

    // Add the UI elements below the viewport
    buffer << Renderer::renderData(inventory);

    // Reset color to default
    buffer << Colorizer::getColorCode(Color::RESET);
    buffer << "FPS:(MAX = 1000) = " + std::to_string(fps);
    // Flush the buffer to the console in one go
    std::cout << buffer.str();
}
void Renderer::initializeMap(std::vector<std::vector<Tile>>& map)
{
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
    for (; Enemy::enemies.size() < std::floor((std::min)((std::min)(Dwarf::Cursor::floor / 2, 20), (WIDTH + HEIGHT) / 4));) {
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
