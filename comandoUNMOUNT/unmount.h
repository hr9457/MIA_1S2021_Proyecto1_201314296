#ifndef UNMOUNT_H
#define UNMOUNT_H

#include "Estructuras/structs.h"
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

class unmount
{
private:
    bool busquedaParticion(vector<montajeDisco>&,string);
    void impresionMontaje(vector<montajeDisco>&);
    void revisionParticionesDiscos(vector<montajeDisco>&);
public:
    unmount();
    void desmontarDisco(vector<montajeDisco>&,string);
};

#endif // UNMOUNT_H