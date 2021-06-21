#include "Registro.h"
#include "LineaRegistro.h"

#include <iostream>

Registro::Registro()
{
    finalizado = false;
    lineas = new Vector<LineaRegistro>();
}

Registro::Registro(const Vector<LineaRegistro>& lineas)
{
    this->lineas = new Vector<LineaRegistro>();
    *(this->lineas) = lineas;
}

Registro::Registro(const Registro& other)
{
    lineas = new Vector<LineaRegistro>();
    for(size_t i = 0; i < other.lineas->Size(); i++)
        lineas->PushBack((*other.lineas)[i]);
    finalizado = other.finalizado;
}

Registro::~Registro()
{
    delete lineas;
}

void Registro::agregarLinea(const LineaRegistro& linea)
{
    lineas->PushBack(linea);
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

std::ostream& operator<<(std::ostream& stream, const Registro& r)
{
    for(LineaRegistro lr : *r.lineas)
        stream << lr << std::endl;

    return stream;
}