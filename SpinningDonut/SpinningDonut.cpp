#include <iostream>
#include <cmath>

using namespace std;

int main() {

    const int R1 = 10, R2 = 3, FOV = 10;
    const int width = 84, height = 26;
    char screen[width][height];

    float theta, phi;

    system("cls");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            screen[x][y] = ' ';
        }
    }

    for (phi = 0; phi < 6.28; phi += 0.02) {
        for (theta = 0; theta < 6.28; theta += 0.02) {

            // X = (R + r*cos(theta)) * cos(phi)
            // Y = (R + r*cos(theta)) * sen(phi)
            // Z = r*sen(theta)

			float X = (R1 + R2 * cos(theta)) * cos(phi); // Need to find his min and max values to scale it to the screen
			float Y = (R1 + R2 * cos(theta)) * sin(phi);
			float Z = R2 * sin(theta) + FOV;

			float projX = (X * FOV) / Z; // Max value when X = R1 + R2, Z = R2 + FOV |  Min value when X = -(R1 + R2), Z = -R2 + FOV 
			float projY = (Y * FOV) / Z; // Max value when Y = R1 + R2, Z = R2 + FOV |  Min value when Y = -(R1 + R2), Z = -R2 + FOV

			float maxProj = (R1 + R2) * FOV / (FOV - R2);

            float scaleX = (width / 2.0) / maxProj;
			float scaleY = (height / 2.0) / maxProj;

			int screenX = static_cast<int>(projX * scaleX + width / 2);
			int screenY = static_cast<int>(projY * scaleY + height / 2);

            if (screenX >= 0 && screenX < width && screenY >= 0 && screenY < height) {
                screen[screenX][screenY] = '|';
            }

        }

    }

    //system("cls");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cout << screen[x][y];
        }
        cout << endl;
    }

    return 0;
}

/*
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <thread>
#include <chrono>

int main() {
    float A = 0, B = 0;
    float i, j;
    float z[1760];
    char b[1760];

    // Limpiar pantalla inicial
    std::cout << "\x1b[2J";
	std::cout << "\x1b[?25l"; // Ocultar cursor

    for (;;) {
        // Inicializar buffers
        std::memset(b, 32, 1760); // Espacios en blanco
        std::memset(z, 0, 7040);  // Z-buffer de profundidad

        // Bucle de renderizado de la superficie del toroide (dona)
        for (j = 0; j < 6.28; j += 0.07) {
            for (i = 0; i < 6.28; i += 0.02) {
                float c = std::sin(i);
                float d = std::cos(j);
                float e = std::sin(A);
                float f = std::sin(j);
                float g = std::cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = std::cos(i);
                float m = std::cos(B);
                float n = std::sin(B);
                float t = c * h * g - f * e;

                // Proyección 2D
                int x = static_cast<int>(40 + 30 * D * (l * h * m - t * n));
                int y = static_cast<int>(12 + 15 * D * (l * h * n + t * m));
                int o = x + 80 * y;

                // Cálculo de iluminación
                int N = static_cast<int>(8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n));

                if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }

        // Mover cursor al inicio de la terminal
        std::cout << "\x1b[H";
        for (int k = 0; k < 1761; k++) {
            std::putchar(k % 80 ? b[k] : 10);
        }

        // Incrementar ángulos de rotación
        A += 0.04f;
        B += 0.02f;

        // Pequeña pausa para no saturar la CPU (opcional en C++)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

	std::cout << "\x1b[?25h"; // Mostrar cursor al salir
    
    return 0;
}
*/