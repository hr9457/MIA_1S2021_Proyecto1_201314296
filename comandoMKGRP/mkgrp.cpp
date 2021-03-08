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
    cout<<"inicio de la particion: "<<part_star<<endl;

    superBloque SP;
    inodo INODO_INICIO;
    bloque_carpetas BLOQUE_BUSQUEDA;

    //------ apertura
    FILE *archivo;
    archivo = fopen(rutaArchivo.c_str(),"rb+");
    if(archivo==NULL)
        exit(1);

    // --- posiciono en el superBloque
    fseek(archivo,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,archivo);


    // ---- inicio de los bloques en la particion
    int inicio_Inodos = SP.s_inode_start;
    cout<<"***Inicio de los inodos "<<inicio_Inodos<<endl;
    int inicio_Bloques = SP.s_block_start;
    cout<<"***Inicio de los blques "<<inicio_Bloques<<endl;
    int inodoDelArchivo;


    // ----- lectura del primer inodo == raiz
    fseek(archivo,part_star,SEEK_SET);
    fread(&INODO_INICIO,sizeof(INODO_INICIO),1,archivo);

   
    cout<<"creacion del nodo raiz"<<endl;
    cout<<INODO_INICIO.i_uid<<endl;
    cout<<INODO_INICIO.i_gid<<endl;
    cout<<INODO_INICIO.i_size<<endl;
    cout<<INODO_INICIO.i_atime<<endl;
    cout<<INODO_INICIO.i_ctime<<endl;
    cout<<INODO_INICIO.i_mtime<<endl;
    for(int i=0;i<15;i++)
    {
        cout<<INODO_INICIO.i_block[i]<<endl;
    }
    cout<<INODO_INICIO.i_type<<endl;
    cout<<INODO_INICIO.i_perm<<endl;


    //--------lo que voy a buscar
    char archivoUser[] = "user.txt";


    // ------ busqueda en cada apuntador directo
    // for(int apuntador=0; apuntador<12; apuntador++)
    // {
    //     if(INODO_INICIO.i_block[apuntador] != -1)
    //     {
    //         // ---- numero de bloque localizado
    //         int numero_bloque = INODO_INICIO.i_block[apuntador];
    //         cout<<"***Numero bloque a revisar"<<numero_bloque<<endl;

    //         // ------ me ubico en ese bloque
    //         if(numero_bloque == 0)
    //         {
    //             cout<<"***Boque cero"<<endl;
    //             //----- bloque cero
    //             fseek(archivo,inicio_Bloques,SEEK_SET);
    //             fread(&BLOQUE_BUSQUEDA,sizeof(BLOQUE_BUSQUEDA),1,archivo);
    //             //------ busqueda en cada apuntador del bloque
    //             for(int i=0;i<4;i++)
    //             {
    //                 cout<<"***Bloque apuntador "<<i<<endl;
    //                 if(strcmp(BLOQUE_BUSQUEDA.b_content[i].b_name,archivoUser)==0)
    //                 {
    //                     //---- inodo donde esta el archivo user.txt
    //                     inodoDelArchivo = BLOQUE_BUSQUEDA.b_content[i].b_inodo;
    //                     break;
    //                 }//fin if

    //             }//fin for
    //             //---- reinicio 
    //             inicio_Bloques = SP.s_block_start;

    //         }// fin del if ==0
    //         else
    //         {

    //             //---- ubicacion en el bloque a revisar
    //             inicio_Bloques = numero_bloque * sizeof(bloque_archivos);
    //             fseek(archivo,inicio_Bloques,SEEK_SET);
    //             fread(&BLOQUE_BUSQUEDA,sizeof(BLOQUE_BUSQUEDA),1,archivo);
    //             // ----- busqueda en cada apuntador de ese bloque
    //             for(int i=0;i<4;i++)
    //             {
    //                 if(strcmp(BLOQUE_BUSQUEDA.b_content[i].b_name,archivoUser))
    //                 {
    //                     inodoDelArchivo = BLOQUE_BUSQUEDA.b_content[i].b_inodo;
    //                     break;
    //                 }
    //             }
    //             //---- reinicio 
    //             inicio_Bloques = SP.s_block_start;

    //         }

    //     }// fin del if verifica que nosea -1
    // }// fin del for principal

    // cout<<"Inodo del archivo: "<<inodoDelArchivo<<endl;

    fclose(archivo);
}



// busqua el numero de inodo del arhivo indicado
void mkgrp::busquedaInodo(int inicio_bloques)
{

}

