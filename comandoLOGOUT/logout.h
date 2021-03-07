#ifndef LOGOUT_H
#define LOGOUT_H

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "Estructuras/structs.h"
using namespace std;
class logout
{
public:
    logout();
    void cerrarSession(vector<usuarioConectado>&);
};

#endif // LOGOUT_H