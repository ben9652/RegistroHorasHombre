#include "Menues.h"
#include "GestorRegistros.h"
#include "LineaRegistro.h"

#include <iostream>
#include <string>
#include <sstream>

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

    LineaRegistro* lr = new LineaRegistro(tiempoSegundos, descripcion);

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
        // Extracci�n de minutos
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
    GestorRegistros* gr = GestorRegistros::instanciar();

    for (unsigned int i = 1; i <= gr->cantidadRegistros(); i++)
    {
        std::string archivoNombreCompleto = archivoNombreIncompleto;
        char* aux = (char*) malloc(6);
        archivoNombreCompleto += itoa(gr->cantidadRegistros(), aux, 10);
        archivoNombreCompleto += ".txt";
        delete aux;

        registro.open(archivoNombreCompleto, std::ios::in);
        if (!registro.fail())
        {
            bool estaFinalizado = false;
            std::string linea;
            while (std::getline(registro, linea))
            {
                if (linea.compare(frase_final))
                    estaFinalizado = true;
            }

            registro.clear();
            registro.seekg(0);

            if (estaFinalizado)
            {

            }
        }
    }
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

    }     while (opcion != 2);
}