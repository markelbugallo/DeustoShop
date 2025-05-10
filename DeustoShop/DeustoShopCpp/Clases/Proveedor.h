#ifndef PROVEEDOR_H
#define PROVEEDOR_H
#include <string>
using namespace std;

class Proveedor
{
private:
    int id_proveedor;
    string nombre_proveedor;
    string contacto_proveedor;
public:
    Proveedor();
    Proveedor(
        int id_proveedor,
        const string &nombre_proveedor,
        const string &contacto_proveedor
    );
    
    int getId_proveedor() const;
    string getNombre_proveedor() const;
    string getContacto_proveedor() const;
};


#endif // PROVEEDOR_H