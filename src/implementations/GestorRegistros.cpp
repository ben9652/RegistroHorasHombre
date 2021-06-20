#include "GestorRegistros.h"
#include "common.h"
#include "Registro.h"

#include <iomanip>
#include <sstream>
#include <string>

GestorRegistros* GestorRegistros::gestor = nullptr;
int GestorRegistros::tiempoTotal = 0;

GestorRegistros::GestorRegistros()
{
    leerRegistros();
}

GestorRegistros::~GestorRegistros()
{
    delete registros;
}

GestorRegistros* GestorRegistros::instanciar()
{
	if (gestor == nullptr)
		gestor = new GestorRegistros();
	return gestor;
}

Codigo GestorRegistros::leerRegistros()
{
    std::ifstream registro;

    for (unsigned int i = 1; i <= cantidadRegistros(); i++)
    {
        std::string nombreArchivo = "Registro ";
        char* aux = (char*) malloc(6);
        nombreArchivo += itoa(cantidadRegistros(), aux, 10);
        nombreArchivo += ".txt";
        delete aux;

        registro.open(nombreArchivo, std::ios::in);
        if (!registro.fail())
        {
            Registro reg_a_agregar;

            std::string linea;
            while (std::getline(registro, linea))
            {
                int TiempoConsumido = 0;
                tm FechaActual{};
                std::string Descripcion = "";

                std::stringstream ssLineas(linea);
                std::string token;
                for (unsigned int i = 0; std::getline(ssLineas, token, ' '); i++)
                {
                    std::stringstream ssDatos(token);
                    switch (i)
                    {
                        case 0:
                            TiempoConsumido = std::stoi(token);
                            break;

                        case 1:
                        {
                            std::string fecha;

                            std::getline(ssDatos, fecha, '/');
                            FechaActual.tm_mday = std::stoi(fecha);

                            std::getline(ssDatos, fecha, '/');
                            FechaActual.tm_mon = std::stoi(fecha);

                            std::getline(ssDatos, fecha, '/');
                            FechaActual.tm_year = std::stoi(fecha) - 1900;

                            break;
                        }
                        case 2:
                        {
                            std::string hora;

                            std::getline(ssDatos, hora, ':');
                            FechaActual.tm_hour = std::stoi(hora);

                            std::getline(ssDatos, hora, ':');
                            FechaActual.tm_min = std::stoi(hora);

                            break;
                        }
                        case 3:
                            continue;

                        default:
                            Descripcion += token;
                            Descripcion += ' ';
                            break;
                    }
                }
                Descripcion.erase(Descripcion.size() - 1, 1);
                Descripcion += '.';

                LineaRegistro linea_a_agregar(TiempoConsumido, FechaActual, Descripcion);
                reg_a_agregar.agregarLinea(linea_a_agregar);
            }
            this->registros->push_back(reg_a_agregar);
        }
        else
            return Codigo::ERROR_APERTURA;
    }
    return Codigo::OK;
}

Codigo GestorRegistros::escribirRegistro(const LineaRegistro& dato)
{
    bool dosHorasSuperadas = false;

    // Es: o el tiempo que falta para completar las 2 horas en el registro actual, o simplemente el tiempo
    // que queremos agregar al registro.
    int tiempoFaltante = dato.getTiempoConsumido();

    // tiempoTotal ser� el tiempo consumido, pero ya en el siguiente registro

    if (tiempoTotal > 7200)
    {
        dosHorasSuperadas = true;
        tiempoTotal -= 7200;
        tiempoFaltante -= tiempoTotal;
    }

    std::ofstream regActual;
    std::string nombre_registro = "Registro ";
    char* aux = (char*) malloc(6);
    nombre_registro += itoa(cantidadRegistros(), aux, 10);
    nombre_registro += ".txt";
    delete aux;

    int tiempoConsumido = tiempoFaltante;
    int dia = dato.getDay();
    int mes = dato.getMonth();
    int anio = dato.getYear();
    int hora = dato.getHour();
    int minuto = dato.getMinutes();
    std::string descripcion = dato.getDescription();

    regActual.open(nombre_registro, std::ios::out | std::ios::app);
    if (!regActual.fail() && tiempoConsumido != 0)
    {
        regActual << tiempoConsumido << ' ';
        regActual << std::setw(2) << std::setfill('0') << dia << '/';
        regActual << std::setw(2) << std::setfill('0') << mes << '/';
        regActual << std::setw(2) << std::setfill('0') << anio << ' ';
        regActual << std::setw(2) << std::setfill('0') << hora << ':';
        regActual << std::setw(2) << std::setfill('0') << minuto << " - ";
        regActual << descripcion << std::endl;
    }

    if (dosHorasSuperadas)
    {
        regActual << std::endl;
        regActual << "2 horas cumplidas -> $ 300";
    }

    regActual.close();

    if (dosHorasSuperadas)
    {
        std::string nombre_registro = "Registro ";
        char* aux = (char*) malloc(6);
        nombre_registro += itoa(cantidadRegistros(), aux, 10);
        nombre_registro += ".txt";
        delete aux;

        tiempoConsumido = obtenerTiempoTotal();

        regActual.open(nombre_registro, std::ios::out);
        if (!regActual.fail())
        {
            regActual << tiempoConsumido << ' ';
            regActual << std::setw(2) << std::setfill('0') << dia << '/';
            regActual << std::setw(2) << std::setfill('0') << mes << '/';
            regActual << std::setw(2) << std::setfill('0') << anio << ' ';
            regActual << std::setw(2) << std::setfill('0') << hora << ':';
            regActual << std::setw(2) << std::setfill('0') << minuto << " - ";
            regActual << descripcion << std::endl;
        }

        regActual.close();
    }

    if (cantidadRegistros() == 0)
    {
        Registro registroNuevo;
        registroNuevo.agregarLinea(dato);

        registros->push_back(registroNuevo);
    }
    else
        obtenerRegistro(cantidadRegistros() - 1).agregarLinea(dato);

    return Codigo::OK;
}

void GestorRegistros::agregarRegistro(const Registro& registro)
{
    registros->push_back(registro);
}

Registro& GestorRegistros::obtenerRegistro(unsigned int index) const
{
    return (*registros)[index];
}

void GestorRegistros::agregarTiempo(int tiempo_a_agregar)
{
    tiempoTotal += tiempo_a_agregar;
}

void GestorRegistros::restarTiempo(int tiempo_a_restar)
{
    tiempoTotal -= tiempo_a_restar;
}

void GestorRegistros::reiniciarTiempo()
{
    tiempoTotal = 0;
}

int GestorRegistros::obtenerTiempoTotal()
{
    return tiempoTotal;
}

size_t GestorRegistros::cantidadRegistros() const
{
    return registros->size();
}