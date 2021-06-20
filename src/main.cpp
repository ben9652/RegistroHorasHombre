#include <Windows.h>

#include "GestorRegistros.h"
#include "Menues.h"

int main()
{
    // Establecer el idioma a español
    std::setlocale(LC_ALL, "spanish");
    SetConsoleCP(1252);                 // Cambiar STDIN

    GestorRegistros* gr = GestorRegistros::instanciar();

    MenuPrincipal menu;

    return 0;
}