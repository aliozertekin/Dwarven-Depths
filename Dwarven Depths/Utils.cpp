#include "Utils.h"


// Function to get a character
char Utils::ccgetch()
{
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
// Function to check for keypress
bool Utils::cckbhit()
{
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

void Utils::clearScreen()
{
#ifdef _WIN32
    // Use ANSI codes to clear screen if enabled
    std::cout << "\033[2J\033[H";
#else
    // Use ANSI codes for Unix/Linux
    std::cout << "\033[2J\033[H";
#endif
    std::cout.flush(); // Ensure the commands are processed immediately
}

void Utils::enableANSI()
{
#ifdef _WIN32
    // Enable ANSI escape codes for Windows
    std::system("clear"); // This sends an empty command to enable ANSI on Windows terminal
#else
    // On Unix/Linux, ANSI codes are supported by default
#endif
}



