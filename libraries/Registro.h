#include <vector>
#include "LineaRegistro.h"

#ifndef _REGISTROS
#define _REGISTROS
struct Registro
{
private:
    std::vector<LineaRegistro>* lineas;
    bool finalizado;
public:
    Registro();
    Registro(const std::vector<LineaRegistro>& lineas);
    Registro(const Registro& other);
    ~Registro();

    void agregarLinea(const LineaRegistro& linea);
    LineaRegistro& obtenerLinea(unsigned int index);

    void finalizar();
    bool estaFinalizado();
};
#endif