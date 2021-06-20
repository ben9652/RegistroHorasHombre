#include "LineaRegistro.h"
#include "Registro.h"
#include "common.h"

#include <vector>
#include <fstream>

#ifndef _GESTOR_REGISTROS
#define _GESTOR_REGISTROS

class GestorRegistros
{
private:
    std::vector<Registro>* registros = new std::vector<Registro>();
    static GestorRegistros* gestor;
    static int tiempoTotal;

    GestorRegistros();
    ~GestorRegistros();

    Codigo leerRegistros();

public:
    static GestorRegistros* instanciar();

    void agregarTiempo(int tiempo_a_agregar);
    void restarTiempo(int tiempo_a_restar);
    void reiniciarTiempo();
    int obtenerTiempoTotal();

    Codigo escribirRegistro(const LineaRegistro& dato);

    void agregarRegistro(const Registro& registro);
    Registro& obtenerRegistro(unsigned int index) const;

    size_t cantidadRegistros() const;
};
#endif