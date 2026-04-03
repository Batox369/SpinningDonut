#include "ScreenHandler.h"
#include <iostream>

using namespace std;

Display& Display::getInstance() {
	static Display instance;
	return instance;
}

Display::Display() {
	clearScreen();
}

void Display::setPixel(int X, int Y, float depth, char symbol) {
	// This function will set a pixel on the screen at a specific coordinate with a specific character and depth value.

	if (X >= 0 && X < WIDTH && Y >= 0 && Y < HEIGHT) {
		if (depth > zBuffer[X][Y]) {
			zBuffer[X][Y] = depth;
			screen[X][Y] = symbol;
		}
	}
}

void Display::clearScreen() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			screen[x][y] = ' ';
			zBuffer[x][y] = 0;
		}
	}
}

void Display::renderScreen() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			cout << screen[x][y];
		}
		cout << endl;
	}
}