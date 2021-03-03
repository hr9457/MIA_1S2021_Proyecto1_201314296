#ifndef RMDISK_H
#define RMDISK_H
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "libreria/funciones.h"
using namespace std;

class rmdisk
{
private:
    string rutaArchivo;
    funciones FUN;
    void confirmacionEliminacion(string);
    void removerDisco(string);
public:
    rmdisk();
    void eliminarDisco(string);    
};

#endif // RMDISK_H
