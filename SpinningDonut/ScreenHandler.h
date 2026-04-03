#pragma once

class Display {
public:
	static Display& getInstance();

	Display(const Display&) = delete;
	void operator=(const Display&) = delete;

	static int getWidth() { return WIDTH; };
	static int getHeight() { return HEIGHT; };

	void setPixel(int X, int Y, float depth, char symbol);
	void clearScreen();
	void renderScreen();
private:
	Display();

	static constexpr int WIDTH = 90, HEIGHT = 26;
	char screen[WIDTH][HEIGHT];
	float zBuffer[WIDTH][HEIGHT];
};