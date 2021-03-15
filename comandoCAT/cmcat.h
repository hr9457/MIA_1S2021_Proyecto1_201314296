#ifndef COMANDOCAT_H
#define COMANDOCAT_H

#include <iostream>
#include <string>
#include <vector>
#include "Estructuras/structs.h"
#include "libreria/funciones.h"
using namespace std;

class cmcat
{
private:
    string parmetro_filen;
    funciones FUN;
public:
    cmcat();
    void leerArchivo(vector<usuarioConectado>&,vector<montajeDisco>&,string);
    bool verificacionUsuario(vector<usuarioConectado>&);
    bool verificacionParametro(string);
    string eliminarComillas(string);
    void buscarArchivoTXT(string,vector<usuarioConectado>&,vector<montajeDisco>&);
};

#endif // COMANDOCAT_H