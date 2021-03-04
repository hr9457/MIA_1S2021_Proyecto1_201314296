#include "mkfs.h"

mkfs::mkfs()
{
}


// funcion principal la creacion del sistemas de archivos
void mkfs::crearSistemaArchivos(string parametros[])
{
    // posiciones de los parametros: identificador - 0, type - 1, fs - 2
    if(parametros[0].empty()!=true)
    {        
        this->identificador=parametros[0];
        // actualizacion de parametros opcionales type, fs
        parametrosOpcionales(parametros);
        cout<<"-->se creara un SA para: "<<identificador<<" type:"<<tipoFormateo<<" SA: "<<tipoSistema<<endl;
    }
    else
    {
        cout<<"-->Parametros obligatorios faltantes: id"<<endl;
    }
}


// funcion para actualizar los parametros opcionesles
void mkfs::parametrosOpcionales(string parametos[])
{
    // revison del parametro tipo type
    if(parametos[1].empty()!=true)
    {
        string tipo = FUN.aMinuscula(parametos[1]);
        if(tipo == "fast")
        {
            this->tipoFormateo = "fast";
        }
        else if(tipo == "full")
        {
            this->tipoFormateo = "full";
        }
        else
        {
            cout<<"-->Tipos de formateo no coincide: uso por defecto full"<<endl;
        }
    }
    // revision del parametro tipo fs
    if(parametos[2].empty()!=true)
    {
        string sistema = FUN.aMinuscula(parametos[2]);
        if(sistema == "2fs")
        {
            this->tipoSistema = "2fs";
        }
        else if(sistema == "3fs")
        {
            this->tipoSistema = "3fs";
        }
        else
        {
            cout<<"-->Tipo de sistema de archivos no coinciden: 2fs o 3fs se uso por defecto EXT2"<<endl;
        }
    }
}
