#ifndef MKGRP_H
#define MKGRP_H

#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include <cstring> 
#include "libreria/funciones.h"
#include "Estructuras/structs.h"
using namespace std;

class mkgrp
{
private:
    string grupoUsser,usser;
    funciones FUN;
public:
    mkgrp();
    void crearGrupo(vector<usuarioConectado>&,string[],vector<montajeDisco>&);
    bool verificacionParametros(string[]);
    string eliminacionComillas(string);
    bool verificacionUsuarioRoot(vector<usuarioConectado>&);
    void busquedaInodo(int);
    void aperturaArchivo(vector<montajeDisco>&,vector<usuarioConectado>&);
};

#endif // MKGRP_H