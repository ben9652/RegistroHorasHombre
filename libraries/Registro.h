#include <iostream>
#include <Vector.h>
#include "LineaRegistro.h"

#ifndef _REGISTROS
#define _REGISTROS
class Registro
{
private:
    Vector<LineaRegistro>* lineas;
    bool finalizado;
public:
    Registro();
    Registro(const Vector<LineaRegistro>& lineas);
    Registro(const Registro& other);
    ~Registro();

    void agregarLinea(const LineaRegistro& linea);
    LineaRegistro& obtenerLinea(size_t index);

    size_t cantidadLineas();

    void finalizar();
    bool estaFinalizado();
    
    friend std::ostream& operator<<(std::ostream& stream, const Registro& r);
};
#endif