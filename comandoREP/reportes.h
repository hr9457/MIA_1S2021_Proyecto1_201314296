#ifndef REPORTES_H
#define REPORTES_H


#include <iostream>
#include <string>
#include <vector>
#include "libreria/funciones.h"
#include "Estructuras/structs.h"
using namespace std;
class reportes
{
private:
    vector<string> reportesPermitidos = {"mbr","disk","inode","journaling","block"
    ,"bm_inode","bm_block","tree","sb","file","ls"};
    string name,path,identificador,ruta;
    funciones FUN;
public:
    reportes();
    void generarReporte(string[]);
    bool parametrosObligatorios(string[]);
    bool verificacionReporte();
};

#endif // REPORTES_H