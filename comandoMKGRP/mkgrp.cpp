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
        this->grupoUsser += "\n";
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

    // --- busco el inodo del archivo de texto en el indo encontrados
    fseek(archivo,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,archivo);
    int inicio_inodo_archivo = SP.s_inode_start;
    int inicio_bloques = SP.s_block_start;
    inicio_inodo_archivo = inicio_inodo_archivo + ( nodoDelArchivo * sizeof(inodo) );

    // cout<<"inicio del inodo users: "<<inicio_inodo_archivo<<endl;
    // --- lectura del inodo econtrado
    inodo inodo_archivo_texto;
    fseek(archivo,inicio_inodo_archivo,SEEK_SET);
    fread(&inodo_archivo_texto,sizeof(inodo_archivo_texto),1,archivo);

    // --- concatenacion del nuevo grupo a insertar
    char nuevoGrupo[] = "1,G,";
    strcat(nuevoGrupo,this->grupoUsser.c_str());
    int longitudNuevo = sizeof(nuevoGrupo) + grupoUsser.size();

    // --- funcion para ir a insertar el nuevo grupo en el bloque existente o en uno nuevo
    string contenido_texto = FUN.concatenarArchivoTexto(archivo,part_star,inodo_archivo_texto,nuevoGrupo,longitudNuevo);
    fclose(archivo);

    // ---- Reelectura del bloque que se escribio 
    // //------ apertura
    // FILE *archivo2;
    // archivo2 = fopen(rutaArchivo.c_str(),"rb+");
    // if(archivo==NULL)
    //     exit(1);
    // // yo se que es el bloque 1
    // bloque_archivos leer;
    // inicio_bloques = inicio_bloques + sizeof(bloque_archivos);
    // fseek(archivo2,inicio_bloques,SEEK_SET);
    // fread(&leer,sizeof(leer),1,archivo2);
    // cout<<leer.b_content<<endl;
    // fclose(archivo2);
}



// busqua el numero de inodo del arhivo indicado
void mkgrp::busquedaInodo(int inicio_bloques)
{

}

