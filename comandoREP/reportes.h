#ifndef REPORTES_H
#define REPORTES_H


#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <math.h> 
#include <cmath>
#include "libreria/funciones.h"
#include "Estructuras/structs.h"
using namespace std;
class reportes
{
private:
    vector<string> reportesPermitidos = {"mbr","disk","inode","journaling","block"
    ,"bm_inode","bm_block","tree","sb","file","ls"};
    string name,path,identificador,ruta;
    string nombreArchivoDot;
    funciones FUN;
public:
    reportes();
    void generarReporte(string[],vector<usuarioConectado>&,vector<montajeDisco>&);
    bool parametrosObligatorios(string[]);
    bool verificacionReporte();
    string eliminacionComillas(string);
    void crearCarpetas(string);
    bool usuarioLogeado(vector<usuarioConectado>&);
    string descomponerRuta(string);
    void generarTipoReporte(string,vector<usuarioConectado>&,vector<montajeDisco>&);
    void reporteMBR(string);
    string tipoConversion(string);
    void reporteDISK();
    void reportBMindode(string,int);
    void reportBMbloque(string,int);
    void reportSP(string,int);
    void reportTree(string,int);
    void reportInode(string,int);
    void reportBlock(string,int);
    void reportJornali(string,int);
    void reportDisk(string);
};

#endif // REPORTES_H