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
    bool espacio_archivo = false;

    //------ apertura
    FILE *archivo;
    archivo = fopen(rutaArchivo.c_str(),"rb+");
    if(archivo==NULL)
        exit(1);
    
    // lectura de superbloque de la particion
    fseek(archivo,part_star,SEEK_SET);
    fread(&SP,sizeof(superBloque),1,archivo);
    int inicio_inodos = SP.s_inode_start;
    int inicio_bloque = SP.s_block_start;
    int primer_bloque_libre = SP.s_first_blo;
    int inicio_bm_bloques = SP.s_bm_block_start;

    // --- datos para el nuevo bloque
    string dato_nuevo_grupo = "1,G,";
    dato_nuevo_grupo += this->grupoUsser;

    //--------lo que voy a buscar
    char archivoUser[] = "user.txt";

    // --- doble impresion de prueba
    int nodoDelArchivo = FUN.buscarInodoArchivoCarpeta(archivo,part_star,archivoUser);

    // ------
    cout<<"Nodo del archivo a buscar es: "<<nodoDelArchivo<<endl;

    // --- busco el inodo del archivo de texto en el indo encontrados
    inicio_inodos = inicio_inodos + (nodoDelArchivo * sizeof(inodo));
    cout<<inicio_inodos<<endl;
    inodo inodo_archivo_usuarios;
    fseek(archivo,inicio_inodos,SEEK_SET);
    fread(&inodo_archivo_usuarios,sizeof(inodo),1,archivo);

    for(int apuntador=0; apuntador<12; apuntador++)
    {        
        if(inodo_archivo_usuarios.i_block[apuntador]!= -1)
        {            
            int espacio = inodo_archivo_usuarios.i_block[apuntador];
            cout<<"Espacio disponible para colocar el nuevo bloque: "<<espacio<<endl;

            bloque_archivos nuevo_texto;
            strcpy(nuevo_texto.b_content,dato_nuevo_grupo.c_str());            

            // escirbo el bloque en el primer bloque libre
            inicio_bloque = inicio_bloque + (primer_bloque_libre * sizeof(bloque_archivos));
            fseek(archivo,inicio_bloque,SEEK_SET);
            fwrite(&nuevo_texto,sizeof(bloque_archivos),1,archivo);

            inodo_archivo_usuarios.i_block[espacio] = primer_bloque_libre;
            inicio_bloque = SP.s_block_start;

            // actualizo el bitmpa de bloques
            char ocupado = '1';
            inicio_bm_bloques = inicio_bm_bloques + (primer_bloque_libre);
            fseek(archivo,inicio_bm_bloques,SEEK_SET);
            fwrite(&ocupado,sizeof(ocupado),1,archivo);
            primer_bloque_libre++;
            espacio_archivo = true;
            break;
        }
    }


    //  actualizacion de inodo
    fseek(archivo,inicio_inodos,SEEK_SET);
    fwrite(&inodo_archivo_usuarios,sizeof(inodo),1,archivo);

    // actualizacion de super bloque de la particion
    SP.s_first_blo = primer_bloque_libre;
    fseek(archivo,part_star,SEEK_SET);
    fwrite(&SP,sizeof(superBloque),1,archivo);

    if(espacio_archivo)
    { 
        cout<<"---> Se ha creado un nuevo grupo para usuarios "<<endl;
        cout<<endl;
    }
    else
    {
        cout<<"---> NO hay espacio en el archiv usr.txt para crea un nuevo grupo de usuarios"<<endl;
        cout<<endl;
    }
    fclose(archivo);
}



// busqua el numero de inodo del arhivo indicado
void mkgrp::busquedaInodo(int inicio_bloques)
{

}

