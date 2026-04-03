#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "ScreenHandler.h"

using namespace std;

const int SIDE = 9;
const int FOV = 40, DISTANCE = 50;
const int PYRAMID_CHUNKS = 10;

const char PYRAMID_CHARS[] = { '.', ',', '-', '~', ':', ';', '=', 'x', '#', '@' };


struct Point3D {
	float x, y, z;
	int faceID; // 0: Front, 1: Back, 2: Right, 3: Left
};

struct UnitVector {
	float x, y, z;
	int brightness;
};

UnitVector facePNormals[6] = {
	{ 0, 0, 1 },   // Front
	{ 0, 0, -1 },  // Back
	{ 0, 1, 0 },   // Top
	{ 0, -1, 0 },  // Bottom
	{ 1, 0, 0 },   // Right
	{ -1, 0, 0 }   // Left
};

const UnitVector LIGTH_VECTOR = { 1.0f, 0, 0 };

vector<Point3D> pyramidPoints;

void makePyramid() {
	float sideWidth, sideHeight, halfSide = SIDE / 2.0;
	float pyramidHeight = SIDE;
	
	for (sideHeight = 0; sideHeight <= pyramidHeight; sideHeight += 0.1f) {
		// We are making squares
		
		float currentSide = (sideHeight / pyramidHeight) * SIDE;
		float currentHalfSide = currentSide / 2.0;

		for (float sideWidth = -currentHalfSide; sideWidth <= currentHalfSide; sideWidth += 0.1f) {
			pyramidPoints.push_back({ sideWidth, sideHeight, currentHalfSide, 0 }); // Front face
			pyramidPoints.push_back({ sideWidth, sideHeight, -currentHalfSide, 1 }); // Back face
			pyramidPoints.push_back({ currentHalfSide, sideHeight, sideWidth, 2 }); // Right face
			pyramidPoints.push_back({ -currentHalfSide, sideHeight, sideWidth, 3 }); // Left face
		}
	}

}

void drawPyramid(float theta) {
	float projX, projY;
	float maxProj = SIDE / 2 * sqrt(3);

	float scaleX = (Display::getWidth() / 2.0) / maxProj;
	float scaleY = (Display::getHeight() / 2.0) / maxProj;

	Display& display = Display::getInstance();

	
	static const UnitVector STATIC_FACE_NORMALS[5] = {
		{ 0.0f,    -0.447f,  0.894f }, 
		{ 0.0f,    -0.447f, -0.894f }, 
		{ 0.894f,  -0.447f,  0.0f   }, 
		{ -0.894f, -0.447f,  0.0f   }, 
		{ 0.0f,     1.0f,    0.0f   }
	};

	for (int i = 0; i < 4; i++) {
		// Rotation in Y-axis for the pyramid normals
		float XTemp = STATIC_FACE_NORMALS[i].x;

		facePNormals[i] = {
			STATIC_FACE_NORMALS[i].x * cos(theta) + STATIC_FACE_NORMALS[i].z * sin(theta),
			STATIC_FACE_NORMALS[i].y,
			-XTemp * sin(theta) + STATIC_FACE_NORMALS[i].z * cos(theta)
		};

		float dot = facePNormals[i].x * LIGTH_VECTOR.x + facePNormals[i].y * LIGTH_VECTOR.y + facePNormals[i].z * LIGTH_VECTOR.z;
		float brightness = (dot + 1) / 2;
		facePNormals[i].brightness = static_cast<int>(brightness * 9);
	}

	for (size_t i = 0; i < pyramidPoints.size(); ++i) {
		float actualX = pyramidPoints[i].x;
		float actualY = pyramidPoints[i].y;
		float actualZ = pyramidPoints[i].z;

		// Rotation in Y-axis for the pyramid points

		float tempX = actualX;

		actualX = actualX * cos(theta) + actualZ * sin(theta);
		actualY = actualY;
		actualZ = -tempX * sin(theta) + actualZ * cos(theta);

		projX = (actualX * FOV) / (actualZ + DISTANCE);
		projY = (actualY * FOV) / (actualZ + DISTANCE);

		float ooz = 1 / (actualZ + DISTANCE);

		int screenX = static_cast<int>(((projX * scaleX) / 1.5) + Display::getWidth() / 2.0);
		int screenY = static_cast<int>((projY * scaleY) + Display::getHeight() / 2.0);
			
		display.setPixel(screenX, screenY, ooz, PYRAMID_CHARS[facePNormals[pyramidPoints[i].faceID].brightness]);

	}

	display.renderScreen();
}

void startRenderingPyramid() {
	float theta = 0.0, phi = 0.0;

	Display& display = Display::getInstance();

	makePyramid();

	system("cls");

	printf("\x1b[2J");
	printf("\x1b[?25l");

	while (true) {
		display.clearScreen();

		theta += 0.05;
		phi += 0.03;

		printf("\x1b[H");

		drawPyramid(theta);

		this_thread::sleep_for(chrono::milliseconds(20));
	}
}