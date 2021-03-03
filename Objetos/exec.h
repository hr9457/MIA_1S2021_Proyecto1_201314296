#ifndef EXEC_H
#define EXEC_H
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <termios.h>
#include <parser.h>
#include <scanner.h>
#include "libreria/funciones.h"
using namespace std;

//exec -path=/home/hector/prueba/calificacion.script

class exec
{
private:
    string rutaArchivo;
    funciones FUN;//funciones externas de ayuda
public:
    exec();
    void leerArchivo(string);
    void cargarArchivo(string);
};

#endif // EXEC_H
