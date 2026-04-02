#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;

const int WIDTH = 90, HEIGHT = 26;
const int SIDE = 6;
const int FOV = 40, DISTANCE = 50;

const char CUBE_CHARS[] = { '.', ',', '-', '~', ':', ';', '=', '#' };

float cubeSide = 0.0;

char screen[WIDTH][HEIGHT];
float zBuffer[WIDTH][HEIGHT];

struct Point3D {
	float x, y, z;
	int faceID; // 0: Front, 1: Back, 2: Top, 3: Bottom, 4: Right, 5: Left
};

struct UnitVector {
	float x, y, z;
	int brightness; // 0 to 7, where 0 is darkest and 7 is brightest
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

const UnitVector LIGTH_VECTOR = { 0, 1, 0 };

void initScreen() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			screen[x][y] = ' ';
			zBuffer[x][y] = 0;
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

	float scaleX = (WIDTH / 2.0) / maxProj;
	float scaleY = (HEIGHT / 2.0) / maxProj;

	for (int i = 0; i < 6; i++) {
		
		const UnitVector STATIC_FACE_NORMALS[6] = {
			{ 0, 0, 1 },   // Front
			{ 0, 0, -1 },  // Back
			{ 0, 1, 0 },   // Top
			{ 0, -1, 0 },  // Bottom
			{ 1, 0, 0 },   // Right
			{ -1, 0, 0 }   // Left
		};

		float YTemp = STATIC_FACE_NORMALS[i].y;

		faceNormals[i] = {
			STATIC_FACE_NORMALS[i].x,
			STATIC_FACE_NORMALS[i].y * cos(theta) - STATIC_FACE_NORMALS[i].z * sin(theta),
			YTemp * sin(theta) + STATIC_FACE_NORMALS[i].z * cos(theta)
		};

		float dot = faceNormals[i].x * LIGTH_VECTOR.x + faceNormals[i].y * LIGTH_VECTOR.y + faceNormals[i].z * LIGTH_VECTOR.z;
		faceNormals[i].brightness = (dot > 0) ? static_cast<int>(dot * 7) : 0;
	}

	for (size_t i = 0; i < cubePoints.size(); ++i) {
		float actualX = cubePoints[i].x;
		float actualY = cubePoints[i].y;
		float actualZ = cubePoints[i].z;

		float tempY = actualY;

		actualX = actualX;
		actualY = actualY * cos(theta) - actualZ * sin(theta);
		actualZ = tempY * sin(theta) + actualZ * cos(theta);

		projX = (actualX * FOV) / (actualZ + DISTANCE);
		projY = (actualY * FOV) / (actualZ + DISTANCE);

		float ooz = 1 / (actualZ + DISTANCE);

		int screenX = static_cast<int>((projX * scaleX) + WIDTH / 2);
		int screenY = static_cast<int>((projY * scaleY) + HEIGHT / 2);

		if (ooz > zBuffer[screenX][screenY]) {
			zBuffer[screenX][screenY] = ooz;
			if (CUBE_CHARS[faceNormals[cubePoints[i].faceID].brightness] > 0) {
				screen[screenX][screenY] = CUBE_CHARS[faceNormals[cubePoints[i].faceID].brightness];
			}
		}
	}

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			cout << screen[x][y];
		}
		cout << endl;
	}
}

void startCubeRendering() {
	
	float theta = 0.0, phi = 0.0;
	
	makeCube();

	system("cls");

	printf("\x1b[2J");
	printf("\x1b[?25l");

	while (true) {
		initScreen();

		theta += 0.05;
		phi += 0.03;

		printf("\x1b[H");

		drawCube(theta, phi);
		cout << "Theta: " << theta << " | Phi: " << phi << endl;

		this_thread::sleep_for(chrono::milliseconds(20));
	}

}