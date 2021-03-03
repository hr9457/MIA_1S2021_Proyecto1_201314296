#ifndef MOUNT_H
#define MOUNT_H

#include "Estructuras/structs.h"
#include "libreria/funciones.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class mount
{
private:
    funciones FUN;
    string rutaArchivo = "";
    const string carnet = "96";
    int numero = 0;
    string letra = "a";
    string identificador;
    int discoMontado = 0;
    char abc[26] = {'a','b','c','d','e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    int contadorLetras;
    //vector<montajeDisco> discos;
public:
    mount();
    void montarDisco(string[],vector<montajeDisco>&,int&);
    string verificacionComillas(string);
    bool verificacionDisco(string);
    void addLista(string,int&,vector<montajeDisco>&,string);
    string generarIdentificador(int&,int,montajeDisco&);
    void impresionListadoParticones(vector<montajeDisco>&);
    bool searchParticionDisco(string,string);
};

#endif // MOUNT_H