#include "LineaRegistro.h"
#include "common.h"
#include "GestorRegistros.h"

#include <string>

LineaRegistro::LineaRegistro(int tConsumido, const std::string& descr)
{
    GestorRegistros* gr = GestorRegistros::instanciar();

    TiempoConsumido = tConsumido;

    gr->agregarTiempo(tConsumido);

    time_t tActual;
    tActual = time(&tActual);
    FechaActual = localtime(&tActual);
    
    Descripcion = descr;
}

LineaRegistro::LineaRegistro(int TiempoConsumido, tm FechaActual, const std::string& Descripcion)
{
    this->TiempoConsumido = TiempoConsumido;
    (*this->FechaActual) = FechaActual;
    this->Descripcion = Descripcion;
}

LineaRegistro::LineaRegistro(const LineaRegistro& other)
    : TiempoConsumido(other.TiempoConsumido),
      Descripcion(other.Descripcion)
{
    FechaActual = other.FechaActual;
}

unsigned int LineaRegistro::getTiempoConsumido() const
{
    return TiempoConsumido;
}

void LineaRegistro::setTiempoConsumido(int TiempoConsumido)
{
    this->TiempoConsumido = TiempoConsumido;
}

unsigned int LineaRegistro::getDay() const
{
    return FechaActual->tm_mday;
}

unsigned int LineaRegistro::getMonth() const
{
    return (FechaActual->tm_mon + 1);
}

unsigned int LineaRegistro::getYear() const
{
    return (FechaActual->tm_year + 1900);
}

unsigned int LineaRegistro::getHour() const
{
    return FechaActual->tm_hour;
}

unsigned int LineaRegistro::getMinutes() const
{
    return FechaActual->tm_min;
}

std::string LineaRegistro::getDescription() const
{
    return Descripcion;
}