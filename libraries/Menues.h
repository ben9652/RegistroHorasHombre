#include <string>
#include <fstream>
#include "LineaRegistro.h"

#ifndef _MENUES
#define _MENUES
struct SeleccionTiempo
{
    std::string tiempo;
    std::string descripcion;
    int tiempoSegundos;
    
    SeleccionTiempo();

private:
    int tiempoIngresadoCorrectamente();
};

struct MostrarPagos
{
    std::ifstream registro;
    const std::string frase_final = "2 horas cumplidas -> $ 300";
    const char* archivoNombreIncompleto = "Registro ";

    MostrarPagos();
};

struct MenuPrincipal
{
    MenuPrincipal();
};
#endif