#include <time.h>
#include <string>

#ifndef _LINEAS_REGISTROS
#define _LINEAS_REGISTROS
struct LineaRegistro
{
private:
    int TiempoConsumido;   // Tiempo (en segundos) consumido en esta instancia
    tm* FechaActual;
    std::string Descripcion;

public:
    LineaRegistro(int tConsumido, const std::string& descr);

    LineaRegistro(int TiempoConsumido, tm FechaActual, const std::string& Descripcion);

    unsigned int getTiempoConsumido() const;

    unsigned int getDay() const;

    unsigned int getMonth() const;

    unsigned int getYear() const;

    unsigned int getHour() const;

    unsigned int getMinutes() const;

    std::string getDescription() const;
};
#endif