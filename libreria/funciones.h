#ifndef FUNCIONES_H
#define FUNCIONES_H
#include "Estructuras/structs.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
using namespace std;
class funciones
{
private:
    string rutaArchivo = "";
public:
    funciones();
    vector<string> split(string,char);
    string eliminacionComillas(string);
    bool is_file(string file);
    string aMayuscula(string);
    string aMinuscula(string);
    void limpiarVector(string []);
    bool buscarElemento(string [],string);
    string vectorAstring(char []);
    bool elementoEnVector(vector<string>,string);
    string obtenerFechaHora();
    bool busquedaParticion(vector<montajeDisco>&,string);
    int busquedaSizeParticion(vector<montajeDisco>&,string);
    int busquedaStarParticion(vector<montajeDisco>&,string);
    string busquedaPathParticion(vector<montajeDisco>&,string);
    string convertirArreglochar(char[]);
    bool buscarDentroVector(string,string);
    int buscarInodoArchivoCarpeta(FILE*,int,char[]);
    int buscarInodoArchivoCarpeta2(FILE*,superBloque&,int,string,int&);
    int espacioLibreBloqueInodo(FILE*,superBloque&,int);
    int espacioLibreBloqueInodo2(FILE*,superBloque&,int);
    int espacioLibreInodo(FILE*,superBloque&,int,int&);
    void lineaAlinea();
    string concatenarArchivoTexto(FILE*,int,inodo,char[],int);
};

#endif // FUNCIONES_H
