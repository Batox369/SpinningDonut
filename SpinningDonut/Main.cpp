#include <iostream>

#include "SpinningDonut.h"
#include "SpinningCube.h"

using namespace std;

char opcion = ' ';

void mostrarOpciones() {
	system("cls");
	cout << "------| FIGURAS 3D |------" << endl;
	cout << "(1). Dona" << endl;
	cout << "(2). Cubo" << endl;
	cout << "(3). Piramide" << endl;
	cout << endl;
	cout << "(0). Salir" << endl;
}

int main() {

	do {
		mostrarOpciones();

		cout << "Ingrese su opcion --> ";
		cin >> opcion;
		switch (opcion) {
			case '1':
				mostrarDona();
				break;
			case '2':
				startCubeRendering();
			default: { }
		}
		
	} while (opcion != '0');

	return 0;
}