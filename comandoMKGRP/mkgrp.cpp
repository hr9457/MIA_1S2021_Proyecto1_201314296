#include "mkgrp.h"

mkgrp::mkgrp()
{}


// metodo para crear un nuevo grupo de usuarios
void mkgrp::crearGrupo(vector<usuarioConectado>&usrConectados,string parametros[],vector<montajeDisco>&listadoDiscos)
{
    if(verificacionParametros(parametros))
    {
        if(verificacionUsuarioRoot(usrConectados))
        {
            aperturaArchivo(listadoDiscos,usrConectados);
        }
        else
        {
            cout<<"--->El usuario con el que ha iniciado session no es un usurio Root<---"<<endl;
        }
    }
    else
    {
        cout<<"--->Parametros Obligatorios faltantes: name"<<endl;
    }
}



// verifica los parametros obligatorios esten
bool mkgrp::verificacionParametros(string parametros[])
{
    if(parametros[0].empty()!=true)
    {
        this->grupoUsser = eliminacionComillas(parametros[0]);
        return true;
    }
    else
    {
        return false;
    }
}


// elimina las comillas
string mkgrp::eliminacionComillas(string palabra)
{
    if(palabra[0]== '\"')
    {
        return FUN.eliminacionComillas(palabra);
    }
    else
    {
        return palabra;
    } 
}


// verifica la conexion de un usurio root
bool mkgrp::verificacionUsuarioRoot(vector<usuarioConectado>&usrConectados)
{
    char usrRoot[] = "root";
    for(int usuario=0; usuario<usrConectados.size(); usuario++)
    {
        if(strcmp( usrConectados[usuario].usuario, usrRoot )==0)
        {
            return true;
        }
    }
    return false;
}




// apertura el archivo para leer la info de la particion
void mkgrp::aperturaArchivo(vector<montajeDisco>&listadoDiscos,vector<usuarioConectado>&usrConetados)
{
    // sacar el id del usurio conectado
    string identificador = FUN.convertirArreglochar(usrConetados[0].identificador);
    string rutaArchivo = FUN.busquedaPathParticion(listadoDiscos,identificador);
    int part_star = FUN.busquedaStarParticion(listadoDiscos,identificador);
    //cout<<"inicio de la particion: "<<part_star<<endl;

    superBloque SP;
    inodo INODO_INICIO;
    bloque_carpetas BLOQUE_BUSQUEDA;

    //------ apertura
    FILE *archivo;
    archivo = fopen(rutaArchivo.c_str(),"rb+");
    if(archivo==NULL)
        exit(1);
    
    //--------lo que voy a buscar
    char archivoUser[] = "user.txt";

    // --- doble impresion de prueba
    int nodoDelArchivo = FUN.buscarInodoArchivoCarpeta(archivo,part_star,archivoUser);

    // ------
    cout<<"Nodo del archivo a buscar es: "<<nodoDelArchivo<<endl;

    fclose(archivo);
}



// busqua el numero de inodo del arhivo indicado
void mkgrp::busquedaInodo(int inicio_bloques)
{

}

