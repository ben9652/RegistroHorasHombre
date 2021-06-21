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
    const std::string frase_final = "2 horas cumplidas -> $ 300";

    MostrarPagos();
};

struct MenuPrincipal
{
    MenuPrincipal();
};
#endif