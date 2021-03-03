#ifndef FDISK_H
#define FDISK_H

#include "Estructuras/structs.h"
#include "libreria/funciones.h"
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
using namespace std;

class fdisk
{
private:
    vector<partition2> copia;
    vector<blackPartition> particionesLibres;
    string rutaArchivo;
    funciones FUN;
    string tamanioParticion = "k";
    string typeParticion = "p";
    string tipoAjuste = "wf";
    char ajuste;    
    string name="",u="k",f,type;
    int sizeParticion;
    char fitParticion='w';
    char tipoParticion='p';
    int contadorPrimarias=0,contadorExtendidas=0,contadorLogicas=0,totalParticiones=0;
public:
    fdisk();
    void ejecutarFdisk(string []);
    bool verificacionDisco(string);
    bool addDelete(string []);
    bool sizeAdd(string[]);
    void crearAgregarParticion(string []);
    void encontrarEspaciosLibres();
    void ordenarEspaciosLibres();
    void buscarDentroParticion(int);
    void ordenDescedente();
    void crearParticion(int);
    void insertarParticion();
    void impresionParticionesLibres();
    void parametrosOPcrear(string []);
    void agregarActualizarMBR(char,char,char,int,int,string);
    void eliminarParticion(string []);
    void eliminarActulizarMBR(string,string);
    string verificacionComillas(string);
};

#endif // FDISK_H

// fdisk -path=/home/hector/prueba/DiscoPrueba.dk -size=200 -add=200 
