#ifndef MKFS_H
#define MKFS_H

#include "libreria/funciones.h"
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
    void crearSistemaArchivos(string[]);
    void parametrosOpcionales(string[]);
};

#endif // MKFS_H