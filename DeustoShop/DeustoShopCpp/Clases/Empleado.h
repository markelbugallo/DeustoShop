#ifndef EMPLEADO_H
#define EMPLEADO_H
#include <string>
using namespace std;

class Empleado {
private:
    int id_empleado;
    string nombre;
    int id_almacen;
    string rol;
    
public:
    Empleado();
    Empleado(
        int id_empleado,
        const string &nombre,
        int id_almacen,
        const string &rol
    );

    // getters
    int getId_empleado() const;
    string getNombre() const;
    int getId_almacen() const;
    string getRol() const;
};

#endif // EMPLEADO_H