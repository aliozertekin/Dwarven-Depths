#pragma once
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#endif

class Utils {
public:
	static char ccgetch();
	static bool cckbhit();
	static void clearScreen();
	static void enableANSI();
};