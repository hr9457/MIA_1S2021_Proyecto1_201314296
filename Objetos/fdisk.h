#ifndef FDISK_H
#define FDISK_H

//librerias
#include <iostream>
#include <string>
#include <cstring>
#include "libreria/funciones.h"
using namespace std;

class fdisk
{
private:
    string rutaArchivo;
    funciones FUN;
    void addDelete(string []);
public:
    fdisk();
    void ejecutarFdisk(string []);
};

#endif // FDISK_H
