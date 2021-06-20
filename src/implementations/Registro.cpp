#include "Registro.h"
#include "LineaRegistro.h"

#include <vector>

Registro::Registro()
{
    finalizado = false;
    lineas = new std::vector<LineaRegistro>();
}

Registro::Registro(const std::vector<LineaRegistro>& lineas)
{
    this->lineas = new std::vector<LineaRegistro>();
    *(this->lineas) = lineas;
}

Registro::Registro(const Registro& other) : lineas(other.lineas)
{
    finalizado = other.finalizado;
}

Registro::~Registro()
{
    delete lineas;
}

void Registro::agregarLinea(const LineaRegistro& linea)
{
    lineas->push_back(linea);
}

LineaRegistro& Registro::obtenerLinea(unsigned int index)
{
    return (*lineas)[index];
}

void Registro::finalizar()
{
    finalizado = true;
}

bool Registro::estaFinalizado()
{
    return finalizado;
}