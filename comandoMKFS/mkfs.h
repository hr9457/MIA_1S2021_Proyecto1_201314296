#ifndef MKFS_H
#define MKFS_H

#include "libreria/funciones.h"
#include "Estructuras/structs.h"
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;
class mkfs
{
private:
    funciones FUN;
    string identificador;
    string tipoFormateo="full";
    string tipoSistema="2fs";
public:
    mkfs();
    void crearSistemaArchivos(string[],vector<montajeDisco>&);
    void parametrosOpcionales(string[]);
    bool busquedaParticion(vector<montajeDisco>&);
    void selecionarSistemaArchivos();
    void formateoFast();
    void formateoFull();
    void sistemaExt2();
    void sistemaExt3();
    void calcularInodos(vector<montajeDisco>&,string);
};

#endif // MKFS_H