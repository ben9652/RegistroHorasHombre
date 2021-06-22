#include "LineaRegistro.h"
#include "Registro.h"
#include "common.h"

#include <time.h>
#include <Vector.h>
#include <fstream>

#ifndef _GESTOR_REGISTROS
#define _GESTOR_REGISTROS

class GestorRegistros
{
private:
    static Vector<Registro>* registros;
    static GestorRegistros* gestor;
    static int tiempoTotal;

    const char* RUTA = "C:\\Users\\Benjamin\\Documents\\Programas\\C++\\RegistroHorasTrabajadas\\Registros\\";
    const int TIEMPO_LIMITE = 7200;

    GestorRegistros();
    ~GestorRegistros();

    Codigo leerRegistros();

public:
    static GestorRegistros* instanciar();

    void agregarTiempo(int tiempo_a_agregar);
    void restarTiempo(int tiempo_a_restar);
    void reiniciarTiempo();
    int obtenerTiempoTotal();
    tm obtenerTmTotal();

    Codigo escribirRegistro(LineaRegistro& dato);

    void agregarRegistro(const Registro& registro);
    Registro& obtenerRegistro(unsigned int index) const;
    void finalizarUltimo();

    size_t cantidadRegistros() const;

    Vector<Registro>* obtenerLista() const;
};
#endif