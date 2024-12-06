# Dwarven Depths

Dwarven Depths is a roguelike game that challenges players to explore dungeons, fight enemies, and collect resources. You play as a Dwarf navigating through various floors of the dungeon while facing dangerous enemies, solving puzzles, and collecting gold and stone to upgrade your abilities. The game runs in a terminal and offers a dynamic gameplay experience.

## Features

- **Dwarf Movement:** Navigate through the dungeon, moving on a grid-like map.
- **Enemies:** Various enemies roam the dungeon, and you must avoid or fight them.
- **Resource Management:** Collect resources like gold and stone to improve your abilities.
- **Floor Progression:** The game features multiple floors, with new challenges and enemies as you progress.
- **Turn-Based Combat:** Enemies move every frame, and combat can be triggered when they collide with the Dwarf.

## Game Setup

### Prerequisites

To build and run Dwarven Depths, you will need:

- **C++17 or later** compiler (e.g., GCC, Clang, or MSVC)
- **CMake** for building the project
- **Terminal/Command Line Interface (CLI)** to run the game

### Building the Game

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/DwarvenDepths.git
    cd DwarvenDepths
    ```

2. Create a build directory:
    ```bash
    mkdir build
    cd build
    ```

3. Run CMake to generate the build files:
    ```bash
    cmake ..
    ```

4. Build the project:
    ```bash
    make
    ```

5. Run the game:
    ```bash
    ./DwarvenDepths
    ```

### Controls

- **W, A, S, D** – Move the Dwarf (Up, Left, Down, Right)
- **Arrow Keys** – Move the cursor to navigate the interface
- **Enter** – Confirm selection or interact
- **Esc** – Exit the game

### Game Flow

1. The game starts on the first floor of the dungeon.
2. You can move the Dwarf using the WASD or arrow keys.
3. You must avoid or defeat enemies as you explore the dungeon.
4. Collect resources like **Gold** and **Stone** as you progress.
5. Move to the next floor once you’ve navigated through the current one.
6. If the Dwarf collides with an enemy, combat will ensue.

### Game Modes

- **Standard Mode**: Explore the dungeon, collect resources, and progress through the floors.
- **Hard Mode**: Enemies are more aggressive, and you must manage resources carefully.

## Code Structure

The game is implemented in C++ and is structured as follows:

- **Main.cpp**: The entry point of the game.
- **Game.h / Game.cpp**: Core game logic, including player movement and floor progression.
- **Renderer.h / Renderer.cpp**: Responsible for rendering the game to the terminal.
- **Dwarf.h / Dwarf.cpp**: Logic related to the Dwarf character, including movement and actions.
- **Enemy.h / Enemy.cpp**: Handles enemy movement and interactions with the Dwarf.
- **Utils.h / Utils.cpp**: Utility functions for time management, clearing the screen, and other miscellaneous tasks.

### Dependencies

The game uses the following libraries:

- **C++ Standard Library** for basic functionality like input/output, containers, and time management.
- **Threading** for smooth, concurrent game execution.
- **ANSI Escape Codes** for clearing the screen and other terminal manipulations.

## Contributing

Contributions are welcome! Feel free to fork the repository, make changes, and create a pull request.

### How to Contribute

1. Fork the repository.
2. Create a new branch for your changes.
3. Make your changes and commit them.
4. Push the changes to your fork.
5. Submit a pull request with a detailed explanation of your changes.

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Thanks to [open source libraries](https://github.com/) and the C++ community for providing powerful tools that made this game possible.

---

**Enjoy the adventure in the Dwarven Depths!** 🚀
