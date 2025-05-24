#include "Utils.h"
#include <iostream>
#include <limits>
using namespace std;

int pedirEntero(const string& mensaje) {
    int valor;
    cout << mensaje;
    cin >> valor;
    if (cin.fail()) {
        cout << "Entrada no válida. Volviendo al menú anterior..." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    return valor;
}