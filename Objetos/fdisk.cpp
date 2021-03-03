#include "fdisk.h"

// metodo constructor
fdisk::fdisk()
{}

// metodo de ejecuccion principal
void fdisk::ejecutarFdisk(string parametros[])
{
    if(parametros[0].empty() != true)
    {
        this->rutaArchivo = parametros[0];
        if(rutaArchivo[0] == '\"')
        {
            this->rutaArchivo = FUN.eliminacionComillas(rutaArchivo);
            // verificacion de la existencia del disco
            if(FUN.is_file(rutaArchivo))
            {
                addDelete(parametros);
            }
            else
            {cout<<"-->No es posible acceder al disco"<<endl;}
        }
        else
        {
            // verificacion de la existencia del disco
            if(FUN.is_file(rutaArchivo))
            {
                addDelete(parametros);
            }
            else
            {cout<<"-->No es posible acceder al disco"<<endl;}
        }
    }
    else
    { cout<<"-->Parametros Obligatorios faltantes: path"<<endl;}
}


// funcion para la verificacion de que los paremetros add y delete no puedan venir juntos 
void fdisk::addDelete(string parametros[])
{
    if(parametros[1].empty() != true && parametros[2].empty() != true)
    {   cout<<"-->Erro: parametros add y delete incompatibles"<<endl;   }
    else
    {   }
}
