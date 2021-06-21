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
    size_t cantidadRegistros = 0;

    struct stat info;
    if(stat("C:\\Users\\Benjamin\\Documents\\Programas\\C++\\RegistroHorasTrabajadas\\Registros", &info) != 0)
        return Codigo::SIN_ARCHIVOS;
    else if(info.st_mode & S_IFDIR)
    {
        std::string archivo;
        size_t contador = 1;
        while(1)
        {
            archivo = RUTA;
            archivo += "Registro ";
            char* aux = (char*) malloc(6);
            archivo += itoa(contador, aux, 10);
            archivo += ".txt";

            aux = nullptr;

            registro.open(archivo, std::ios::in);
            if(registro.fail()) break;
            else cantidadRegistros++;

            registro.close();

            contador++;
        }
    }
    else
        return Codigo::ERROR_APERTURA;
    
    for (unsigned int i = 0; i < cantidadRegistros; i++)
    {
        std::string nombreArchivo = RUTA;
        nombreArchivo += "Registro ";
        char* aux = (char*) malloc(6);
        nombreArchivo += itoa(i + 1, aux, 10);
        nombreArchivo += ".txt";
        delete aux;

        registro.open(nombreArchivo, std::ios::in);
        if (!registro.fail())
        {
            Registro* reg_a_agregar = new Registro();

            std::string linea;
            while (std::getline(registro, linea))
            {
                if(linea == "") break;
                int TiempoConsumido = 0;
                tm* FechaActual = (tm*)malloc(sizeof(tm));
                std::string Descripcion = "";

                const char* constDescr = Descripcion.c_str();

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
                            FechaActual->tm_mday = std::stoi(fecha);

                            std::getline(ssDatos, fecha, '/');
                            FechaActual->tm_mon = std::stoi(fecha);

                            std::getline(ssDatos, fecha, '/');
                            FechaActual->tm_year = std::stoi(fecha) - 1900;

                            break;
                        }
                        case 2:
                        {
                            std::string hora;

                            std::getline(ssDatos, hora, ':');
                            FechaActual->tm_hour = std::stoi(hora);

                            std::getline(ssDatos, hora, ':');
                            FechaActual->tm_min = std::stoi(hora);

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

                char* descripcion_posta = (char*)malloc(strlen(constDescr) + 1);
                memcpy(descripcion_posta, constDescr, strlen(constDescr) + 1);

                // Si es el último registro, que se vaya agregando el tiempo total
                if(i == cantidadRegistros - 1)
                    tiempoTotal += TiempoConsumido;

                LineaRegistro* linea_a_agregar = new LineaRegistro(TiempoConsumido, FechaActual, descripcion_posta);
                reg_a_agregar->agregarLinea(*linea_a_agregar);
            }
            this->registros->PushBack(*reg_a_agregar);
        }
        else
            return Codigo::ERROR_APERTURA;

        registro.close();

        if(i != cantidadRegistros - 1)
            finalizarUltimo();
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

        // Al cumplirse ya el tiempo de trabajo de 2 horas, se finaliza el registro para pasar a crear otro desde cero.
        finalizarUltimo();

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

void GestorRegistros::finalizarUltimo()
{
    (*registros)[registros->Size() - 1].finalizar();
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

tm GestorRegistros::obtenerTmTotal()
{
    tm tiempo;
    int segundos;
    int minutos;
    int horas;

    double minutosFloat = tiempoTotal;
    minutosFloat /= 60;
    double horasFloat = minutosFloat /= 60;

    horas = horasFloat;
    if(horasFloat != (double)horas)
    {
        minutosFloat = horasFloat - horas;
        minutosFloat *= 60;
        minutos = minutosFloat;
    }

    minutos = minutosFloat;
    if(minutosFloat != (double)minutos)
    {
        double segundosFloat = minutosFloat - minutos;
        segundosFloat *= 60;
        segundos = segundosFloat;
    }

    tiempo.tm_hour = horas;
    tiempo.tm_min = minutos;
    tiempo.tm_sec = segundos;

    return tiempo;
}

size_t GestorRegistros::cantidadRegistros() const
{
    return registros->Size();
}

Vector<Registro>* GestorRegistros::obtenerLista() const
{
    return registros;
}