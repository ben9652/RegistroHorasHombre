#include <iostream>
#include <fstream>
#include <time.h>

#ifndef _LINEAS_REGISTROS
#define _LINEAS_REGISTROS
class LineaRegistro
{
private:
    int TiempoConsumido;   // Tiempo (en segundos) consumido en esta instancia
    tm* FechaActual;
    char* Descripcion;

public:
    LineaRegistro(int tConsumido, char* descr);

    LineaRegistro(int TiempoConsumido, tm* FechaActual, char* Descripcion);

    LineaRegistro(const LineaRegistro& other);

    ~LineaRegistro();

    unsigned int getTiempoConsumido() const;

    void setTiempoConsumido(int TiempoConsumido);

    unsigned int getDay() const;

    unsigned int getMonth() const;

    unsigned int getYear() const;

    unsigned int getHour() const;

    unsigned int getMinutes() const;

    char* getDescription() const;

    LineaRegistro& operator=(const LineaRegistro& lr);

    friend std::ostream& operator<<(std::ostream& stream, const LineaRegistro& lr);

    friend std::ofstream& operator<<(std::fstream& stream, const LineaRegistro& lr);
};
#endif