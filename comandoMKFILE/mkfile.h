#ifndef MKFILE_H
#define MKFILE_H

#include <iostream>
#include <string>
#include <string.h>
#include "Estructuras/structs.h"
#include "libreria/funciones.h"
using namespace std;

class mkfile
{
private:
    string rutaArchivo,crearCarpetas;
    funciones FUN;
public:
    mkfile();
    void crear(string[]);
    string verificacionComillas(string);
};

#endif // MKFILE_H