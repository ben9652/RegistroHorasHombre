#include "Menues.h"
#include "GestorRegistros.h"
#include "LineaRegistro.h"

#include <iostream>
#include <string>
#include <sstream>
#include <string.h>
#include <ctype.h>

SeleccionTiempo::SeleccionTiempo()
{
    GestorRegistros* gr = GestorRegistros::instanciar();

    system("cls");

    bool datoCorrecto = true;
    do
    {
        if (!datoCorrecto)
            std::cout << "Formato incorrecto." << std::endl;

        datoCorrecto = true;

        std::cout << "Tiempo: ";
        std::cin >> tiempo;

        if ((tiempoSegundos = tiempoIngresadoCorrectamente()) == -1)
            datoCorrecto = false;
    }     while (!datoCorrecto);

    system("cls");

    std::cout << "Descripci\xa2n: ";
    fgetc(stdin);
    std::getline(std::cin, descripcion);

    const char* constDescr = descripcion.c_str();
    char* descripcion_posta = (char*) malloc(strlen(constDescr) + 1);
    memcpy(descripcion_posta, constDescr, strlen(constDescr) + 1);

    LineaRegistro* lr = new LineaRegistro(tiempoSegundos, descripcion_posta);

    gr->escribirRegistro(*lr);
}

int SeleccionTiempo::tiempoIngresadoCorrectamente()
{
    std::stringstream ss(tiempo);
    std::string token;

    int resultado = -1;
    int minutos = 0;
    int segundos = 0;

    size_t posicion_m;
    size_t posicion_s;
    if ((tiempo.find('m') != std::string::npos) && (tiempo.find('s') != std::string::npos))
    {
        // Extracción de minutos
        std::getline(ss, token, 'm');
        for (unsigned int i = 0; i < token.size(); i++)
        {
            if (!isdigit(token[i])) return -1;
        }
        minutos = stoi(token);

        std::getline(ss, token, 'm');
        {
            if (posicion_s = token.find('s'))
                token = token.erase(posicion_s, 1);
        }

        for (unsigned int i = 0; i < token.size(); i++)
        {
            if (!isdigit(token[i])) return -1;
        }
        segundos = stoi(token);

        segundos += minutos * 60;
        resultado = segundos;
    }
    else if ((posicion_m = tiempo.find('m')) != std::string::npos)
    {
        token = token.erase(posicion_m, 1);
        for (unsigned int i = 0; i < token.size(); i++)
        {
            if (!isdigit(token[i])) return -1;
        }

        resultado = stoi(token) * 60;
    }
    else if ((posicion_s = tiempo.find('s')) != std::string::npos)
    {
        token = token.erase(posicion_s, 1);
        for (unsigned int i = 0; i < token.size(); i++)
        {
            if (!isdigit(token[i])) return -1;
        }

        resultado = stoi(token);
    }
    else
    {
        for (unsigned int i = 0; i < tiempo.size(); i++)
        {
            if (!isdigit(tiempo[i])) return -1;
        }

        resultado = stoi(tiempo);
    }

    return resultado;
}

MostrarPagos::MostrarPagos()
{
    system("cls");
    GestorRegistros* gr = GestorRegistros::instanciar();
    
    size_t contadorRegistros = 0;
    for(Registro r : *gr->obtenerLista())
    {
        contadorRegistros++;
        std::cout << "Registro " << contadorRegistros << ": ";
        if(r.estaFinalizado())
        {
            std::cout << frase_final << "\n\t";
            std::cout << "Finaliz\xa2 con la siguiente actividad:\n\t\t" << r.obtenerLinea(r.cantidadLineas() - 1) << "\n";
        }
        else
        {
            std::cout << "\n\tNo finaliz\xa2. Lleva ";
            tm tiempo = gr->obtenerTmTotal();
            std::cout << tiempo.tm_hour << " h " << tiempo.tm_min << " min " << tiempo.tm_sec << " seg.\n";
        }
        std::cout << std::endl;
    }

    fgetc(stdin);
    std::cin.get();
}

MenuPrincipal::MenuPrincipal()
{
    unsigned int opcion;
    do
    {
        system("cls");
        std::cout << "1- Ingresar tiempo\n2- Salir\n3- Mostrar pagos" << std::endl;

        std::cin >> opcion;
        if (opcion == 1)
            SeleccionTiempo st;
        else if (opcion == 3)
            MostrarPagos mp;

    } while (opcion != 2);
}