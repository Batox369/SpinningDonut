#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

#include "ScreenHandler.h"

using namespace std;

const int SIDE = 6;
const int FOV = 40, DISTANCE = 50;

const char CUBE_CHARS[] = { '.', ',', '-', '~', ':', ';', '=', 'x', '#', '@'};

float cubeSide = 0.0;

struct Point3D {
	float x, y, z;
	int faceID; // 0: Front, 1: Back, 2: Top, 3: Bottom, 4: Right, 5: Left
};

struct UnitVector {
	float x, y, z;
	int brightness;
};

vector<Point3D> cubePoints;

UnitVector faceNormals[6] = {
	{ 0, 0, 1 },   // Front
	{ 0, 0, -1 },  // Back
	{ 0, 1, 0 },   // Top
	{ 0, -1, 0 },  // Bottom
	{ 1, 0, 0 },   // Right
	{ -1, 0, 0 }   // Left
};

const UnitVector LIGTH_VECTOR = { -0.707f, 0.707f, 0 };

void makeCube() {

	// Every face of the cube will be made by iterating over a grid of points on that face.
	// For example, for the front face, we can iterate over a grid of points where x and y vary from -SIZE to SIZE, and z is constant (e.g., z = SIZE).
	// A point of the front face will be (faceWidth, faceHeight, z), where Z is constant and faceWidth and faceHeight vary from -SIZE to SIZE.

	
	// Memory optimization

	cubePoints.clear();
	cubePoints.reserve(6 * (SIDE / 0.1 + 1) * (SIDE / 0.1 + 1));

	float sideWidth, sideHeight, halfSide = SIDE / 2.0;

	for (sideWidth = -halfSide; sideWidth <= halfSide; sideWidth += 0.02) {
		for (sideHeight = -halfSide; sideHeight <= halfSide; sideHeight += 0.02) {
				
			cubePoints.push_back({ sideWidth, sideHeight, halfSide, 0 }); // Front face
			cubePoints.push_back({ sideWidth, sideHeight, -halfSide, 1 }); // Back face

			cubePoints.push_back({ sideWidth, halfSide, sideHeight, 2 }); // Top face
			cubePoints.push_back({ sideWidth, -halfSide, sideHeight, 3 }); // Bottom face
			
			cubePoints.push_back({ halfSide, sideHeight, sideWidth, 4 }); // Right face
			cubePoints.push_back({ -halfSide, sideHeight, sideWidth, 5 }); // Left face
		}
	}
}

void drawCube(float theta, float phi) {
	// This function will be responsible for drawing the cube on the screen.
	// It will iterate over the points of the cube, apply the necessary transformations (like rotation), and then project them onto a 2D plane to display on the console.

	float projX, projY;
	float maxProj = SIDE / 2 * sqrt(3);

	float scaleX = (Display::getWidth() / 2.0) / maxProj;
	float scaleY = (Display::getHeight() / 2.0) / maxProj;

	Display& display = Display::getInstance();

	// This loop will calculate the normal vectors for each face of the cube 
	// after applying the rotations defined by theta and phi.
	// 
	// The normal vectors are essential for determining how light interacts 
	// with the surfaces of the cube, which in turn affects the brightness 
	// of each face when rendered on the screen.

	for (int i = 0; i < 6; i++) {
		
		const UnitVector STATIC_FACE_NORMALS[6] = {
			{ 0, 0, 1 },   // Front
			{ 0, 0, -1 },  // Back
			{ 0, 1, 0 },   // Top
			{ 0, -1, 0 },  // Bottom
			{ 1, 0, 0 },   // Right
			{ -1, 0, 0 }   // Left
		};
				
		// Rotation in X-asis for the cube normals

		float YTemp = STATIC_FACE_NORMALS[i].y;

		faceNormals[i] = {
			STATIC_FACE_NORMALS[i].x,
			STATIC_FACE_NORMALS[i].y * cos(theta) - STATIC_FACE_NORMALS[i].z * sin(theta),
			YTemp * sin(theta) + STATIC_FACE_NORMALS[i].z * cos(theta)
		};

		// Rotation in Y-asis for the cube normals
		
		float XTemp = faceNormals[i].x;

		faceNormals[i] = {
			faceNormals[i].x * cos(phi) + faceNormals[i].z * sin(phi),
			faceNormals[i].y,
			-XTemp * sin(phi) + faceNormals[i].z * cos(phi)
		};

		float dot = faceNormals[i].x * LIGTH_VECTOR.x + faceNormals[i].y * LIGTH_VECTOR.y + faceNormals[i].z * LIGTH_VECTOR.z;
		float brightness = (dot + 1) / 2;
		faceNormals[i].brightness = static_cast<int>(brightness * 9);
	}

	// This loop will iterate over each point of the cube, apply the necessary transformations,
	// and then project them onto a 2D plane to display on the console.

	for (size_t i = 0; i < cubePoints.size(); ++i) {
		float actualX = cubePoints[i].x;
		float actualY = cubePoints[i].y;
		float actualZ = cubePoints[i].z;

		// Rotation in X-asis for the cube points

		float tempY = actualY;

		actualX = actualX;
		actualY = actualY * cos(theta) - actualZ * sin(theta);
		actualZ = tempY * sin(theta) + actualZ * cos(theta);

		// Rotation in Y-asis for the cube points

		float tempX = actualX;

		actualX = actualX * cos(phi) + actualZ * sin(phi);
		actualY = actualY;
		actualZ = -tempX * sin(phi) + actualZ * cos(phi);

		// Projection of the 3D point to the 2D screen

		projX = (actualX * FOV) / (actualZ + DISTANCE);
		projY = (actualY * FOV) / (actualZ + DISTANCE);

		float ooz = 1 / (actualZ + DISTANCE);

		int screenX = static_cast<int>(((projX * scaleX)/1.5) + Display::getWidth() / 2.0);
		int screenY = static_cast<int>((projY * scaleY) + Display::getHeight() / 2.0);

		display.setPixel(screenX, screenY, ooz, CUBE_CHARS[faceNormals[cubePoints[i].faceID].brightness]);

	}

	// The actual render of the screen

	display.renderScreen();
}

void startCubeRendering() {
	
	float theta = 0.0, phi = 0.0;

	Display& display = Display::getInstance();
	
	makeCube();

	system("cls");

	printf("\x1b[2J");
	printf("\x1b[?25l");

	while (true) {
		display.clearScreen();

		theta += 0.05;
		phi += 0.03;

		printf("\x1b[H");

		drawCube(theta, phi);

		this_thread::sleep_for(chrono::milliseconds(20));
	}

}