#ifndef MKFS_H
#define MKFS_H

#include "libreria/funciones.h"
#include "Estructuras/structs.h"
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;
class mkfs
{
private:
    funciones FUN;
    string identificador;
    string tipoFormateo="full";
    string tipoSistema="2fs";
    int tamanioJournaling = sizeof(journal);
    int totalInodos;
    int totalJournals;
    int tamanioJournal;
    int totalBloques;
    int inicioParticion;
    int tamanioParticion;
    char bmOcupado = '1';
    char bmDisponible = '0';
public:
    mkfs();
    void crearSistemaArchivos(string[],vector<montajeDisco>&);
    void parametrosOpcionales(string[]);
    bool busquedaParticion(vector<montajeDisco>&);
    void formatearParticion(vector<montajeDisco>&,string);
    void selecionarSistemaArchivos(vector<montajeDisco>&,string,int);
    void formateoFast();
    void formateoFull();
    void sistemaExt2(vector<montajeDisco>&,string);
    void sistemaExt3(vector<montajeDisco>&,string);
    void calcularInodos(vector<montajeDisco>&,string,int);
};

#endif // MKFS_H