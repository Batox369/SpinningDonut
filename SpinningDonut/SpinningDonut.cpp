#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>


using namespace std;

int main() {

    const int R1 = 10, R2 = 3, FOV = 10, DISTANCE = 15;
    const int width = 90, height = 26;
    const char DONUT_CHARS[] = {'.', ',', '-', '~', ':', ';', '#'};
    char screen[width][height];
	float zBuffer[width][height];

    float theta, phi;

    // RotX

    float rotX = 0, rotY = 0;

    system("cls");

    printf("\x1b[2J");
    printf("\x1b[?25l");

    while (true)
    {
		rotX += 0.0;
		rotY += 0.3;

        printf("\x1b[H");

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                screen[x][y] = ' ';
				zBuffer[x][y] = 0;
            }
        }

        for (phi = 0; phi < 6.28; phi += 0.02) {
            for (theta = 0; theta < 6.28; theta += 0.02) {

                // -- The parametric equation of the donut is

                // X = (R + r*cos(theta)) * cos(phi)
                // Y = (R + r*cos(theta)) * sen(phi)
                // Z = r*sen(theta)

                float X = (R1 + R2 * cos(theta)) * cos(phi); // Need to find his min and max values to scale it to the screen
                float Y = (R1 + R2 * cos(theta)) * sin(phi);
                float Z = R2 * sin(theta);

				float rotatedX = X;
				float rotatedY = cos(rotX) * Y - sin(rotX) * Z;
				float rotatedZ = sin(rotX) * Y + cos(rotX) * Z;

				float tempRotatedX = rotatedX;

                rotatedX = cos(rotY) * tempRotatedX + sin(rotY) * rotatedZ;
				rotatedY = rotatedY;
				rotatedZ = -sin(rotY) * tempRotatedX + cos(rotY) * rotatedZ;

                X = rotatedX;
                Y = rotatedY;
				Z = rotatedZ;

                Z += DISTANCE;

				float ooz = 1 / Z; // "One over Z" is used to calculate the depth of the point and to scale the projection

                float projX = (X * FOV) / Z; // Max value when X = R1 + R2, Z = R2 + FOV |  Min value when X = -(R1 + R2), Z = -R2 + FOV 
                float projY = (Y * FOV) / Z; // Max value when Y = R1 + R2, Z = R2 + FOV |  Min value when Y = -(R1 + R2), Z = -R2 + FOV

                float maxProj = (R1 + R2) * FOV / (FOV - R2);

                float scaleX = (width / 2.0) / maxProj;
                float scaleY = (height / 2.0) / maxProj;

                int screenX = static_cast<int>((projX * scaleX + width / 2) / 1.5);
                int screenY = static_cast<int>(projY * scaleY + height / 2);

                // Calculating the normal vector of the surface at the point (theta, phi) and adding light

                float nX = cos(theta) * cos(phi);
                float nY = cos(theta) * sin(phi);
                float nZ = sin(theta);

				float rotatedNX = nX;
				float rotatedNY = cos(rotX) * nY - sin(rotX) * nZ;
				float rotatedNZ = sin(rotX) * nY + cos(rotX) * nZ;

                float tempRotatedNX = rotatedNX;

				rotatedNX = cos(rotY) * tempRotatedNX + sin(rotY) * rotatedNZ;
				rotatedNY = rotatedNY;
				rotatedNZ = -sin(rotY) * tempRotatedNX + cos(rotY) * rotatedNZ;

				nX = rotatedNX;
				nY = rotatedNY;
				nZ = rotatedNZ;

                // Were assuming light (0,0,-1)
                float lightEmision = -nZ * 7;

                if (screenX >= 0 && screenX < width && screenY >= 0 && screenY < height) {
                    if (ooz > zBuffer[screenX][screenY]) {
                        zBuffer[screenX][screenY] = ooz;
                        if (lightEmision > 0) {
                            int charIndex = static_cast<int>(lightEmision);
                            screen[screenX][screenY] = DONUT_CHARS[charIndex];
                        }
                    }
                }
            }

        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cout << screen[x][y];
            }
            cout << endl;
        }

		this_thread::sleep_for(chrono::milliseconds(20));
    }

    return 0;
}