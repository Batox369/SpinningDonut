#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

#include "ScreenHandler.h"

using namespace std;

const int R1 = 10, R2 = 3, FOV = 40, DISTANCE = 50;
const char DONUT_CHARS[] = { '.', ',', '-', '~', ':', ';', '=', '#' };

void makeDonut(float rotX, float rotZ) {

	// The donut is defined by two parameters: 
    //  - R1: The distance from the center of the tube to the center of the torus
    //  - R2: Tthe radius of the tube.
    // 
	// We will iterate over the angles phi and theta to compute the 3D coordinates of points on the surface of the torus.


    Display& display = Display::getInstance();

    float maxProj = (R1 + R2) * FOV / (float)(FOV - R2);
    float scaleX = (Display::getWidth() / 2.0f) / maxProj;
    float scaleY = (Display::getHeight() / 2.0f) / maxProj;

    for (float phi = 0; phi < 6.28f; phi += 0.02f) {
        for (float theta = 0; theta < 6.28f; theta += 0.02f) {

            // Donut Equation in 3D space
            float X = (R1 + R2 * cos(theta)) * cos(phi);
            float Y = (R1 + R2 * cos(theta)) * sin(phi);
            float Z = R2 * sin(theta);

            // Rotate around X axis
            float rotatedX = X;
            float rotatedY = cos(rotX) * Y - sin(rotX) * Z;
            float rotatedZ = sin(rotX) * Y + cos(rotX) * Z;

            // Rotate around Z axis
            float tempX = rotatedX;
            rotatedX = cos(rotZ) * tempX - sin(rotZ) * rotatedY;
            rotatedY = sin(rotZ) * tempX + cos(rotZ) * rotatedY;

            X = rotatedX;
            Y = rotatedY;
            Z = rotatedZ + DISTANCE;

            float ooz = 1.0f / Z;

            // Project 3D point onto 2D screen
            int screenX = static_cast<int>((X * FOV / Z * scaleX / 1.5f) + Display::getWidth() / 2.0f);
            int screenY = static_cast<int>((Y * FOV / Z * scaleY) + Display::getHeight() / 2.0f);

            // In this part we compute the normal vector of the surface at the current point, and then rotate it according to the same angles as the point itself.
            // 
            // Why are we making this?
            // 
            // Because we want to compute the lighting intensity based on the angle between the normal 
            // vector and the light source, which is fixed in our case. The light source is assumed to 
            // be coming from the viewer's direction, so we can use the Z component of the normal 
            // vector to determine how much light is hitting that point on the surface.

            float nX = cos(theta) * cos(phi);
            float nY = cos(theta) * sin(phi);
            float nZ = sin(theta);

            // Rotate normal around X axis
            float rotatedNY = cos(rotX) * nY - sin(rotX) * nZ;
            float rotatedNZ = sin(rotX) * nY + cos(rotX) * nZ;

            // Rotate normal around Z axis
            float tempNX = nX;
            nX = cos(rotZ) * tempNX - sin(rotZ) * rotatedNY;
            nZ = rotatedNZ;

            // Compute lighting intensity from normal's Z component
            float lightEmision = -nZ * 8.0f;

            if (lightEmision > 0) {
                int charIndex = static_cast<int>(lightEmision);
                if (charIndex > 7) charIndex = 7;
                display.setPixel(screenX, screenY, ooz, DONUT_CHARS[charIndex]);
            }
        }
    }
}

void drawDonut() {
    Display& display = Display::getInstance();
    display.renderScreen();
}

void startRenderingDonut() {
    float rotX = 0.0f, rotZ = 0.0f;

    Display& display = Display::getInstance();

    system("cls");
    printf("\x1b[2J");
    printf("\x1b[?25l");

    while (true) {
        display.clearScreen();

        rotX += 0.04f;
        rotZ += 0.02f;

        printf("\x1b[H");

        makeDonut(rotX, rotZ);
        drawDonut();

        this_thread::sleep_for(chrono::milliseconds(20));
    }
}