#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int WIDTH = 90, HEIGHT = 26;
const int SIDE = 6;
const int FOV = 40, DISTANCE = 50;

float cubeSide = 0.0;

char screen[WIDTH][HEIGHT];

struct Point3D {
	float x, y, z;
};

vector<Point3D> cubePoints;

void initScreen() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			screen[x][y] = ' ';
		}
	}
}

void makeCube() {

	// Every face of the cube will be made by iterating over a grid of points on that face.
	// For example, for the front face, we can iterate over a grid of points where x and y vary from -SIZE to SIZE, and z is constant (e.g., z = SIZE).
	// A point of the front face will be (faceWidth, faceHeight, z), where Z is constant and faceWidth and faceHeight vary from -SIZE to SIZE.

	
	// Memory optimization

	cubePoints.clear();
	cubePoints.reserve(6 * (SIDE / 0.1 + 1) * (SIDE / 0.1 + 1));

	float sideWidth, sideHeight, halfSide = SIDE / 2.0;

	for (sideWidth = -halfSide; sideWidth <= halfSide; sideWidth += 0.1) {
		for (sideHeight = -halfSide; sideHeight <= halfSide; sideHeight += 0.1) {

			cubePoints.push_back({ sideWidth, sideHeight, halfSide }); // Front face
			cubePoints.push_back({ sideWidth, sideHeight, -halfSide }); // Back face

			cubePoints.push_back({ sideWidth, halfSide, sideHeight }); // Top face
			cubePoints.push_back({ sideWidth, -halfSide, sideHeight }); // Bottom face
			
			cubePoints.push_back({ halfSide, sideHeight, sideWidth }); // Right face
			cubePoints.push_back({ -halfSide, sideHeight, sideWidth }); // Left face
		}
	}
}

void drawCube() {
	// This function will be responsible for drawing the cube on the screen.
	// It will iterate over the points of the cube, apply the necessary transformations (like rotation), and then project them onto a 2D plane to display on the console.

	float projX, projY;
	float maxProj = SIDE / 2 * sqrt(3);

	for (size_t i = 0; i < cubePoints.size(); ++i) {
		projX = (cubePoints[i].x * FOV) / (cubePoints[i].z + DISTANCE);
		projY = (cubePoints[i].y * FOV) / (cubePoints[i].z + DISTANCE);

		float scaleX = (WIDTH / 2.0) / maxProj;
		float scaleY = (HEIGHT / 2.0) / maxProj;

		int screenX = static_cast<int>((projX * scaleX) + WIDTH / 2);
		int screenY = static_cast<int>((projY * scaleY) + HEIGHT / 2);

		screen[screenX][screenY] = '#';
	}
}

void startCubeRendering() {
	initScreen();
	makeCube();
	drawCube();
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			cout << screen[x][y];
		}
		cout << endl;
	}
	system("pause");
}