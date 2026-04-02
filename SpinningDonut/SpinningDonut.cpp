#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;

void mostrarDona() {

    const int R1 = 10, R2 = 3, FOV = 40, DISTANCE = 50;
    const int width = 90, height = 26;

    const char DONUT_CHARS[] = { '.', ',', '-', '~', ':', ';', '=', '#' };

    char screen[width][height];
    float zBuffer[width][height];

    float theta, phi;

    // RotX y RotZ
    float rotX = 0, rotZ = 0;

    system("cls");

    printf("\x1b[2J");
    printf("\x1b[?25l");

    while (true)
    {
        rotX += 0.04;
        rotZ += 0.02;

        printf("\x1b[H");

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                screen[x][y] = ' ';
                zBuffer[x][y] = 0;
            }
        }

        for (phi = 0; phi < 6.28; phi += 0.02) {
            for (theta = 0; theta < 6.28; theta += 0.02) {

                float X = (R1 + R2 * cos(theta)) * cos(phi);
                float Y = (R1 + R2 * cos(theta)) * sin(phi);
                float Z = R2 * sin(theta);

                // --- ROTACIÓN X (Local) ---
                float rotatedX = X;
                float rotatedY = cos(rotX) * Y - sin(rotX) * Z;
                float rotatedZ = sin(rotX) * Y + cos(rotX) * Z;

                // --- ROTACIÓN Z (Global) ---
                float tempRotatedX = rotatedX;
                rotatedX = cos(rotZ) * tempRotatedX - sin(rotZ) * rotatedY;
                rotatedY = sin(rotZ) * tempRotatedX + cos(rotZ) * rotatedY;
                rotatedZ = rotatedZ; // El eje Z no cambia en rotación Z

                X = rotatedX;
                Y = rotatedY;
                Z = rotatedZ;

                Z += DISTANCE;

                float ooz = 1 / Z;

                float projX = (X * FOV) / Z;
                float projY = (Y * FOV) / Z;

                float maxProj = (R1 + R2) * FOV / (FOV - R2);

                float scaleX = (width / 2.0) / maxProj;
                float scaleY = (height / 2.0) / maxProj;

                int screenX = static_cast<int>(((projX * scaleX / 1.5) + width / 2));
                int screenY = static_cast<int>(projY * scaleY + height / 2);

                // --- NORMALES ---
                float nX = cos(theta) * cos(phi);
                float nY = cos(theta) * sin(phi);
                float nZ = sin(theta);

                // Rotación X en Normal
                float rotatedNX = nX;
                float rotatedNY = cos(rotX) * nY - sin(rotX) * nZ;
                float rotatedNZ = sin(rotX) * nY + cos(rotX) * nZ;

                // Rotación Z en Normal
                float tempRotatedNX = rotatedNX;
                rotatedNX = cos(rotZ) * tempRotatedNX - sin(rotZ) * rotatedNY;
                rotatedNY = sin(rotZ) * tempRotatedNX + cos(rotZ) * rotatedNY;
                rotatedNZ = rotatedNZ;

                nX = rotatedNX;
                nY = rotatedNY;
                nZ = rotatedNZ;

                float lightEmision = -nZ * 8;

                if (screenX >= 0 && screenX < width && screenY >= 0 && screenY < height) {
                    if (ooz > zBuffer[screenX][screenY]) {
                        zBuffer[screenX][screenY] = ooz;
                        if (lightEmision > 0) {
                            int charIndex = static_cast<int>(lightEmision);
                            if (charIndex > 7) charIndex = 7; // Evitar que rompa si el índice sube
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
}