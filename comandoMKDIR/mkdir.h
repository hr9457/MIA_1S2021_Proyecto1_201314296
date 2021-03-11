#ifndef MKDIR_H
#define MKDIR_H

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "Estructuras/structs.h"
#include "libreria/funciones.h"
using namespace std;

class mkdir
{
private:
    string rutaArchivo;
    funciones FUN;
public:
    mkdir();
    void crearCarpeta(vector<usuarioConectado>&,string[]);
    bool parametroObligatorio(string[]);
    string eliminacionComillas(string);
    bool verificacionSession(vector<usuarioConectado>&);
    bool parametrosOpcional(string[]);
};

#endif // MKDIR_H