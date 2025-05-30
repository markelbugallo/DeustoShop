#include "Logger.h"
#include <fstream>
#include <ctime>
#include <iostream>
#include <string.h>
using namespace std;

void log(const std::string& mensaje, const std::string& origen) {
    std::ofstream logFile("DeustoShop/DeustoShop/Log/log.txt", std::ios::app);
    if (!logFile) {
        logFile.open("Log/log.txt", std::ios::app);
    }
    if (!logFile) {
        std::cerr << "No se pudo abrir el archivo de log en ninguna ruta." << std::endl;
        return;
    }
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char fechaHora[32];
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", ltm);
    logFile << "[" << origen << "] [" << fechaHora << "] " << mensaje << std::endl;
    cout << mensaje << endl; // Tambien imprimir en consola;
}