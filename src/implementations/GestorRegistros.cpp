#include "GestorRegistros.h"
#include "common.h"
#include "Registro.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <string.h>
#include <ctype.h>

Vector<Registro>* GestorRegistros::registros = new Vector<Registro>();
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

    struct stat info;

    if(stat(RUTA, &info) != 0)
        return Codigo::SIN_ARCHIVOS;
    else if(info.st_mode & S_IFDIR)
    {
        std::string archivo = RUTA;
        archivo += "Registro 1.txt";
        registro.open(archivo, std::ios::in);
        if(registro.fail()) return Codigo::SIN_ARCHIVOS;
    }
    else
        return Codigo::ERROR_APERTURA;
    
    for (unsigned int i = 1; i <= cantidadRegistros(); i++)
    {
        std::string nombreArchivo = RUTA;
        nombreArchivo += "Registro ";
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

                const char* constDescr = Descripcion.c_str();
                char* descripcion_posta = (char*)malloc(strlen(constDescr) + 1);
                memcpy(descripcion_posta, constDescr, strlen(constDescr) + 1);

                LineaRegistro linea_a_agregar(TiempoConsumido, FechaActual, descripcion_posta);
                reg_a_agregar.agregarLinea(linea_a_agregar);
            }
            this->registros->PushBack(reg_a_agregar);
        }
        else
            return Codigo::ERROR_APERTURA;
    }
    return Codigo::OK;
}

Codigo GestorRegistros::escribirRegistro(LineaRegistro& dato)
{
    bool dosHorasSuperadas = false;

    // Es: o el tiempo que falta para completar las 2 horas en el registro actual, o simplemente el tiempo
    // que queremos agregar al registro.
    int tiempoFaltante = dato.getTiempoConsumido();

    // tiempoTotal será el tiempo consumido, pero ya en el siguiente registro

    if (tiempoTotal > 7200)
    {
        dosHorasSuperadas = true;
        tiempoTotal -= 7200;
        tiempoFaltante -= tiempoTotal;
    }
    
    // Realizo una instancia de un archivo de salida
    std::ofstream regActual;

    // Le doy el nombre al registro a escribir
    std::string nombre_registro = RUTA;
    nombre_registro += "Registro ";
    char* aux = (char*) malloc(6);
    size_t x = registros->Size();
    nombre_registro += x == 0? itoa(x + 1, aux, 10) : itoa(x, aux, 10);
    nombre_registro += ".txt";
    delete aux;
    
    LineaRegistro* linea_a_escribir = new LineaRegistro(dato);
    linea_a_escribir->setTiempoConsumido(tiempoFaltante);

    // Reviso si existe la carpeta 'Registros'. De no ser así, la creo
    struct stat info;
    if(stat("C:\\Users\\Benjamin\\Documents\\Programas\\C++\\RegistroHorasTrabajadas\\Registros", &info) != 0)
        _mkdir("C:\\Users\\Benjamin\\Documents\\Programas\\C++\\RegistroHorasTrabajadas\\Registros");

    bool exito_apertura = false;
    // Se intenta hacer la apertura del archivo para escribir al final de este.
    // Si se logra abrir el archivo correctamente, se procede a escribir la línea en él.
    regActual.open(nombre_registro, std::ios::out | std::ios::app);
    if (!regActual.fail() && linea_a_escribir->getTiempoConsumido() != 0)
    {
        exito_apertura = true;
        regActual << *linea_a_escribir << std::endl;
    }

    // Si se llegan a superar las 2 horas predispuestas para el pago, se escribe un mensaje al final.
    if (dosHorasSuperadas & exito_apertura)
    {
        regActual << std::endl;
        regActual << "2 horas cumplidas -> $ 300";
    }

    regActual.close();

    dato.setTiempoConsumido(tiempoFaltante);
    // Si el vector de registros está vacío, se agrega un registro, y se agrega la línea actual en él.
    if (x == 0)
    {
        Registro* registroNuevo = new Registro();
        registroNuevo->agregarLinea(dato);

        registros->PushBack(*registroNuevo);
    }
    // Si hay ya algún registro, simplemente se agrega la línea al último registro.
    else
        obtenerRegistro(x - 1).agregarLinea(dato);

    // Si se superan las dos horas del registro actual con el tiempo agregado, y además
    // se pudo hacer una escritura correcta en el archivo, se ejecuta este condicional.
    if (dosHorasSuperadas & exito_apertura)
    {
        // Se crea una instancia de un archivo de salida
        std::ofstream regActual;

        // Se obtiene la dirección del archivo donde escribir
        std::string nombre_registro = RUTA;
        nombre_registro += "Registro ";
        char* aux = (char*) malloc(6);
        size_t x = registros->Size();
        nombre_registro += itoa(x + 1, aux, 10);
        nombre_registro += ".txt";
        delete aux;

        // Se obtiene el tiempo restante a escribir
        linea_a_escribir->setTiempoConsumido(tiempoTotal);

        regActual.open(nombre_registro, std::ios::out);
        if (!regActual.fail())
            regActual << *linea_a_escribir << std::endl;

        regActual.close();

        // Se agrega la línea del tiempo que sobra a un nuevo registro en el vector de registros
        Registro* registroNuevo = new Registro();
        registroNuevo->agregarLinea(*linea_a_escribir);

        registros->PushBack(*registroNuevo);
    }

    return Codigo::OK;
}

void GestorRegistros::agregarRegistro(const Registro& registro)
{
    registros->PushBack(registro);
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
    return registros->Size();
}