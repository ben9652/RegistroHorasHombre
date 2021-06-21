#include "LineaRegistro.h"
#include "common.h"
#include "GestorRegistros.h"

#include <iomanip>
#include <string>

LineaRegistro::LineaRegistro(int tConsumido, char* descr)
{
    GestorRegistros* gr = GestorRegistros::instanciar();

    TiempoConsumido = tConsumido;

    gr->agregarTiempo(tConsumido);

    time_t tActual;
    tActual = time(&tActual);
    FechaActual = localtime(&tActual);
    
    Descripcion = descr;
}

LineaRegistro::LineaRegistro(int TiempoConsumido, tm FechaActual, char* Descripcion)
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

LineaRegistro::~LineaRegistro()
{
    TiempoConsumido = 0;
    FechaActual = nullptr;
    Descripcion = nullptr;
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

char* LineaRegistro::getDescription() const
{
    return Descripcion;
}

LineaRegistro& LineaRegistro::operator=(const LineaRegistro& lr)
{
    this->TiempoConsumido = lr.TiempoConsumido;
    this->FechaActual = lr.FechaActual;
    this->Descripcion = lr.Descripcion;
    
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const LineaRegistro& lr)
{
    stream << lr.TiempoConsumido << ' ';
    // Con setw() formateo el dato para que se muestren dos dígitos solamente
    // Con setfill() relleno los dígitos restantes de la izquierda
    stream << std::setw(2) << std::setfill('0') << lr.getDay() << '/';
    stream << std::setw(2) << std::setfill('0') << lr.getMonth() << '/';
    stream << std::setw(2) << std::setfill('0') << lr.getYear() << ' ';
    stream << std::setw(2) << std::setfill('0') << lr.getHour() << ':';
    stream << std::setw(2) << std::setfill('0') << lr.getMinutes() << " - ";
    stream << lr.Descripcion;

    return stream;
}

std::ofstream& operator<<(std::ofstream& stream, const LineaRegistro& lr)
{
    stream << lr.getTiempoConsumido() << ' ';
    // Con setw() formateo el dato para que se muestren dos dígitos solamente
    // Con setfill() relleno los dígitos restantes de la izquierda
    stream << std::setw(2) << std::setfill('0') << lr.getDay() << '/';
    stream << std::setw(2) << std::setfill('0') << lr.getMonth() << '/';
    stream << std::setw(2) << std::setfill('0') << lr.getYear() << ' ';
    stream << std::setw(2) << std::setfill('0') << lr.getHour() << ':';
    stream << std::setw(2) << std::setfill('0') << lr.getMinutes() << " - ";
    stream << lr.getDescription();

    return stream;
}