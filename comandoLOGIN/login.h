#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <string>
#include <strings.h>
#include <vector>
#include "Estructuras/structs.h"
#include "libreria/funciones.h"
using namespace std;

class login
{
private:
    string usuario,password,identificador;
    funciones FUN;
public:
    login();
    void iniciarSession(vector<usuarioConectado>&,vector<montajeDisco>&,string[]);
    bool verificacionParametros(string[]);
    string eliminacionComillas(string);
    void verificacionUsuario(vector<montajeDisco>&,string,string,string,vector<usuarioConectado>&);
    void addUsuario(vector<usuarioConectado>&,string,string,string);
};

#endif // LOGIN_H