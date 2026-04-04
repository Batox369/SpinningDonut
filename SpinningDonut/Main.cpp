#include <iostream>

#include "SpinningDonut.h"
#include "SpinningCube.h"
#include "SpinningPyramid.h"

using namespace std;

char opcion = ' ';

void mostrarOpciones() {
	system("cls");
	cout << "------| SPINNING GEOMETRY |------" << endl;
	cout << "(1). Donut" << endl;
	cout << "(2). Cube" << endl;
	cout << "(3). Pyramid" << endl;
	cout << endl;
	cout << "(0). EXIT" << endl;
}

int main() {

	do {
		mostrarOpciones();

		cout << "OPTION --> ";
		cin >> opcion;
		switch (opcion) {
			case '1':
				startRenderingDonut();
				break;
			case '2':
				startCubeRendering();
				break;
			case '3':
				startRenderingPyramid();
				break;
			default: { }
		}
		
	} while (opcion != '0');

	return 0;
}