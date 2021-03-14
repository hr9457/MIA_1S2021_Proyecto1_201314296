#ifndef MKDIR_H
#define MKDIR_H

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "Estructuras/structs.h"
#include "libreria/funciones.h"
using namespace std;

class comamkdir
{
private:
    string rutaArchivo;
    string valorP="false";
    funciones FUN;
public:
    comamkdir();
    void crearCarpeta(vector<usuarioConectado>&,vector<montajeDisco>&,string[]);
    bool parametroObligatorio(string[]);
    string eliminacionComillas(string);
    bool verificacionSession(vector<usuarioConectado>&);
    void parametrosOpcional(string[]);
    void buscarCrearCarpeta(vector<usuarioConectado>&,vector<montajeDisco>&);
};

#endif // MKDIR_H