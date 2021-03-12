#include "mkfs.h"

mkfs::mkfs()
{
}


// funcion principal la creacion del sistemas de archivos
void mkfs::crearSistemaArchivos(string parametros[],vector<montajeDisco>&listado)
{
    // posiciones de los parametros: identificador - 0, type - 1, fs - 2
    if(parametros[0].empty()!=true)
    {        
        this->identificador=parametros[0];
        // actualizacion de parametros opcionales type, fs
        parametrosOpcionales(parametros);
        if(FUN.busquedaParticion(listado,this->identificador))
        {
            cout<<"-->Particion montada<--"<<endl;
            cout<<"-->Formateando Particion ****************"<<endl;
            formatearParticion(listado,this->identificador);
            selecionarSistemaArchivos(listado,this->identificador,this->tamanioJournaling);
            //selecionarSistemaArchivos();
            //calcularInodos(listado,this->identificador);
        }
        else
        {
            cout<<"-->Particion no esta montada aun"<<endl;
        }
        //cout<<FUN.obtenerFechaHora()<<endl;
        //cout<<"-->se creara un SA para: "<<identificador<<" type:"<<tipoFormateo<<" SA: "<<tipoSistema<<endl;
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



// formataeo de la particion antes de la instalcion del sistema de archivos
void mkfs::formatearParticion(vector<montajeDisco>&listado,string identificador)
{
    int part_star = FUN.busquedaStarParticion(listado,identificador);
    int part_size = FUN.busquedaSizeParticion(listado,identificador);
    string rutaArchivo = FUN.busquedaPathParticion(listado,identificador);
    int byteAformatear = part_star;
    
    FILE *archivo;
    // apertura del disco para lectura y actualizacion rb+
    archivo = fopen(rutaArchivo.c_str(),"rb+");    
    if(archivo==NULL)
        exit(1);

    // posiciono al inicio del archivo
    fseek(archivo,part_star,SEEK_SET); // inicio de la particion a formatear
    // re-escribo el para formatear dentro de la particion
    if(this->tipoFormateo == "full")
    {
        char formatearCon = '\0';
        // recorro byte por byte hasta llegar al final de la particion
        for(int numeroByte=0; numeroByte<part_size; numeroByte++)
        {
            // formateo el byte
            fwrite(&formatearCon,sizeof(formatearCon),1,archivo);
            // posiciono en el siguiente byte - byteAformater = inicio de la particion
            fseek(archivo,byteAformatear++,SEEK_SET);
        }
    }
    else if(this->tipoFormateo == "fast")
    {}
    // cierre del dico con los cambios
    fclose(archivo);
}


// funcion para selecionar que sistema de archivos se instalar en la particion
void mkfs::selecionarSistemaArchivos(vector<montajeDisco>&listado,string identificador,int jour)
{
    if(this->tipoSistema == "2fs")
    {
        cout<<"-->se ha instalado el sistemas de archivos ext2"<<endl;
        calcularInodos(listado,identificador,0);
        sistemaExt2(listado,identificador);
    }
    else if(this->tipoSistema == "3fs")
    {
        cout<<"-->se ha instalado el sistema de archivos ext3"<<endl;
        calcularInodos(listado,identificador,jour);
        sistemaExt3(listado,identificador);
    }    
}


// funcion pra instalar sistema ext2
void mkfs::sistemaExt2(vector<montajeDisco>&listado,string identificador)
{
    // inicios para los inodos y bloques a escribir
    int inicioParaInodo,inicioParaBMInodo;
    int inicioParaBloque,inicioParaBMBloque;
    // busco la ruta del arhivo
    string rutaArchivo = FUN.busquedaPathParticion(listado,identificador);
    int part_star = FUN.busquedaStarParticion(listado,identificador);
    FILE *archivo;
    // apertura del disco para lectura y actualizacion rb+
    archivo = fopen(rutaArchivo.c_str(),"rb+");    
    if(archivo==NULL)
        exit(1);

    // Creacion del superBloque para la particion
    superBloque SP;
    SP.s_filesystem_tyep = 2;
    SP.s_inodes_count = totalInodos;
    SP.s_blocks_count = totalBloques;
    SP.s_free_blocks_count = this->totalBloques-2;
    SP.s_free_inodes_count =  this->totalInodos-2;
    strcpy(SP.s_mtime,FUN.obtenerFechaHora().c_str());
    strcpy(SP.s_umtime,FUN.obtenerFechaHora().c_str());
    SP.s_mnt_count = 0;
    SP.s_magic = 0xEF53;
    SP.s_inode_size = sizeof(inodo);
    SP.s_block_size = sizeof(bloque_archivos);
    SP.s_firts_ino = 0;
    SP.s_first_blo = 0;
    SP.s_bm_inode_start = this->inicioParticion + sizeof(superBloque);
    SP.s_bm_block_start = SP.s_bm_inode_start + this->totalInodos;
    SP.s_inode_start =  SP.s_bm_block_start + (3 * this->totalInodos);
    SP.s_block_start = SP.s_inode_start + (this->totalInodos * sizeof(inodo));

    // posicion en el archivo
    fseek(archivo,part_star,SEEK_SET);
    // escrictura del super bloque
    fwrite(&SP,sizeof(SP),1,archivo);
    //---->datos para los inicios de los bloque y inodos nuevos a escribir
    inicioParaInodo = SP.s_inode_start;
    inicioParaBMInodo = SP.s_bm_inode_start;
    inicioParaBloque = SP.s_block_start;
    inicioParaBMBloque = SP.s_bm_block_start;


    // ---- llenado del bitmap de inodos
    int inicio_bm_inodos = SP.s_bm_inode_start;
    int final_bm_inodos = SP.s_bm_block_start - SP.s_bm_inode_start;
    char ocupado = '0';
    fseek(archivo,inicio_bm_inodos,SEEK_SET);
    for(int x=0;x<final_bm_inodos;x++)
    {
        fwrite(&ocupado,sizeof(ocupado),1,archivo);
        inicio_bm_inodos++;
        fseek(archivo,inicio_bm_inodos,SEEK_SET);
    }
    // ---- llenado del bitmap de bloques
    int inicio_de_bloques = SP.s_bm_block_start;
    int final_bm_bloques = SP.s_inode_start - SP.s_bm_block_start;
    fseek(archivo,inicio_de_bloques,SEEK_SET);
    for(int y=0;y<final_bm_bloques;y++)
    {
        fwrite(&ocupado,sizeof(ocupado),1,archivo);
        inicio_de_bloques++;
        fseek(archivo,inicio_de_bloques,SEEK_SET);
    }

    //----- creacion del inodo para para la carpeta root
    inodo raiz;
    raiz.i_uid = 1;
    raiz.i_gid = 1;
    raiz.i_size = 0;
    strcpy(raiz.i_atime,FUN.obtenerFechaHora().c_str());
    strcpy(raiz.i_ctime,FUN.obtenerFechaHora().c_str());
    strcpy(raiz.i_mtime,FUN.obtenerFechaHora().c_str());
    for(int i=0; i<15; i++)
    {
        raiz.i_block[i] = -1;
    }
    raiz.i_type = 0;
    raiz.i_perm = 664;
    // primer apuntador hacia un bloque 
    raiz.i_block[0] = 0;

    //-----Bloque para la carpeta raiz
    bloque_carpetas blockCarpetaRaiz;
    content contentCarpetaRaiz;
    // bloque actual
    strcpy(contentCarpetaRaiz.b_name,".");
    contentCarpetaRaiz.b_inodo = 0;
    blockCarpetaRaiz.b_content[0] = contentCarpetaRaiz;
    // padre
    strcpy(contentCarpetaRaiz.b_name,"..");
    contentCarpetaRaiz.b_inodo = 0;
    blockCarpetaRaiz.b_content[1] = contentCarpetaRaiz;
    // bloque para el archivo user.txt
    strcpy(contentCarpetaRaiz.b_name,"user.txt");
    contentCarpetaRaiz.b_inodo = 1;
    blockCarpetaRaiz.b_content[2] = contentCarpetaRaiz;
    // bloque vacio
    strcpy(contentCarpetaRaiz.b_name,"");
    contentCarpetaRaiz.b_inodo = -1;
    blockCarpetaRaiz.b_content[3] = contentCarpetaRaiz;

    //----Escribo el InodoRaiz 0
    fseek(archivo,inicioParaInodo,SEEK_SET);
    inicioParaInodo = inicioParaInodo + sizeof(inodo);
    fwrite(&raiz,sizeof(raiz),1,archivo);
    fseek(archivo,inicioParaBMInodo,SEEK_SET);//bitmap
    inicioParaBMInodo++;
    fwrite(&bmOcupado,sizeof(bmOcupado),1,archivo);//bitmap

    //----Escribo el bloque 0
    fseek(archivo,inicioParaBloque,SEEK_SET);
    inicioParaBloque = inicioParaBloque + sizeof(bloque_archivos);
    fwrite(&blockCarpetaRaiz,sizeof(blockCarpetaRaiz),1,archivo);
    fseek(archivo,inicioParaBMBloque,SEEK_SET);//bitmap
    inicioParaBMBloque++;
    fwrite(&bmOcupado,sizeof(bmOcupado),1,archivo);//bitmap



    //------INODO PARA USERT.TXT
    char datosarchivo[]="1,G,root\n1,U,root,root,123\n";//cadena a quemar en el archivo
    inodo inodoUsers;
    inodoUsers.i_uid = 1;
    inodoUsers.i_gid = 1;
    inodoUsers.i_size = sizeof(datosarchivo);
    strcpy(inodoUsers.i_atime,FUN.obtenerFechaHora().c_str());
    strcpy(inodoUsers.i_ctime,FUN.obtenerFechaHora().c_str());
    strcpy(inodoUsers.i_mtime,FUN.obtenerFechaHora().c_str());
    for(int i=0; i<15; i++)
    {
        inodoUsers.i_block[i] = -1;
    }
    inodoUsers.i_type = 1;
    inodoUsers.i_perm = 664;
    //apuntador
    inodoUsers.i_block[0] = 1;
    //----Creacion del bloque 1
    bloque_archivos archivoUser;
    strcpy(archivoUser.b_content,datosarchivo);

    //------Escritura del Inodouser 1
    fseek(archivo,inicioParaInodo,SEEK_SET);
    fwrite(&inodoUsers,sizeof(inodoUsers),1,archivo);
    //bitmap inodo
    fseek(archivo,inicioParaBMInodo,SEEK_SET);
    fwrite(&bmOcupado,sizeof(bmOcupado),1,archivo);

    //----Escritura del Bloque
    fseek(archivo,inicioParaBloque,SEEK_SET);
    fwrite(&archivoUser,sizeof(archivoUser),1,archivo);
    //bitmap bloque
    fseek(archivo,inicioParaBMBloque,SEEK_SET);
    fwrite(&bmOcupado,sizeof(bmOcupado),1,archivo);

    // cierre del archivo
    fclose(archivo);
}




// funcion para instalar sistema ext3
void mkfs::sistemaExt3(vector<montajeDisco>&listado,string identificador)
{
    // inicios para los inodos y bloques a escribir
    int inicioParaInodo,inicioParaBMInodo;
    int inicioParaBloque,inicioParaBMBloque;
    // busco la ruta del arhivo
    string rutaArchivo = FUN.busquedaPathParticion(listado,identificador);
    int part_star = FUN.busquedaStarParticion(listado,identificador);
    FILE *archivo;
    // apertura del disco para lectura y actualizacion rb+
    archivo = fopen(rutaArchivo.c_str(),"rb+");    
    if(archivo==NULL)
        exit(1);
    
    // Creacion del superBloque para la particion
    superBloque SP;
    SP.s_filesystem_tyep = 3;
    SP.s_inodes_count = this->totalInodos;
    SP.s_blocks_count = this->totalBloques;
    SP.s_free_blocks_count = this->totalBloques-2;
    SP.s_free_inodes_count =  this->totalInodos-2;
    strcpy(SP.s_mtime,FUN.obtenerFechaHora().c_str());
    strcpy(SP.s_umtime,FUN.obtenerFechaHora().c_str());
    SP.s_mnt_count = 0;
    SP.s_magic = 0xEF53;
    SP.s_inode_size = sizeof(inodo);
    SP.s_block_size = sizeof(bloque_archivos);
    SP.s_firts_ino = 0;
    SP.s_first_blo = 0;
    SP.s_bm_inode_start = this->inicioParticion + sizeof(superBloque) + (this->totalInodos * sizeof(journal));
    SP.s_bm_block_start = SP.s_bm_inode_start + this->totalInodos;
    SP.s_inode_start =  SP.s_bm_block_start + this->totalBloques;
    SP.s_block_start = SP.s_inode_start + (this->totalInodos * sizeof(inodo));

    // posicion en el archivo
    fseek(archivo,part_star,SEEK_SET);
    // escrictura del super bloque
    fwrite(&SP,sizeof(SP),1,archivo);
    //---->datos para los inicios de los bloque y inodos nuevos a escribir
    inicioParaInodo = SP.s_inode_start;
    int inicioParaInodo2 = SP.s_inode_start;
    inicioParaBMInodo = SP.s_bm_inode_start;
    inicioParaBloque = SP.s_block_start;
    inicioParaBMBloque = SP.s_bm_block_start;



    // ----- Escritura del JOURNALING - Registro de la raiz
    int inicioJournal = part_star + sizeof(superBloque);
    journal journalRaiz;
    strcpy(journalRaiz.Journal_Tipo_Operacion,"mkfs");
    journalRaiz.Journal_tipo = '0';
    strcpy(journalRaiz.Journal_nombre,"/");
    strcpy(journalRaiz.Journal_contenido,"-");
    strcpy(journalRaiz.Journal_fecha,FUN.obtenerFechaHora().c_str()); 
    strcpy(journalRaiz.Journal_propietario,"1");
    journalRaiz.Journal_tipo = 664;
    // ----- Escirtura de estos journal sobre la particion
    fseek(archivo,inicioJournal,SEEK_SET);
    inicioJournal = inicioJournal + sizeof(journal);
    fwrite(&journalRaiz,sizeof(journalRaiz),1,archivo);

    //-------- Registro 
    journal journalUsers;
    strcpy(journalUsers.Journal_Tipo_Operacion,"mkfs");
    journalUsers.Journal_tipo = '1';
    strcpy(journalUsers.Journal_nombre,"users.txt");
    strcpy(journalUsers.Journal_contenido,"1,G,root\n1,U,root,root,123\n");
    strcpy(journalUsers.Journal_fecha,FUN.obtenerFechaHora().c_str());
    strcpy(journalUsers.Journal_propietario,"1");
    journalUsers.Journal_tipo = 664;
    //-------Escritura de journal sobre archivo user.txt
    fseek(archivo,inicioJournal,SEEK_SET);
    inicioJournal = inicioJournal + sizeof(journal);
    fwrite(&journalUsers,sizeof(journalUsers),1,archivo);

    // ---- llenado del bitmap de inodos
    int inicio_bm_inodos = SP.s_bm_inode_start;
    int final_bm_inodos = SP.s_bm_block_start - SP.s_bm_inode_start;
    char ocupado = '0';
    fseek(archivo,inicio_bm_inodos,SEEK_SET);
    for(int x=0;x<final_bm_inodos;x++)
    {
        fwrite(&ocupado,sizeof(ocupado),1,archivo);
        inicio_bm_inodos++;
        fseek(archivo,inicio_bm_inodos,SEEK_SET);
    }
    // ---- llenado del bitmap de bloques
    int inicio_de_bloques = SP.s_bm_block_start;
    int final_bm_bloques = SP.s_inode_start - SP.s_bm_block_start;
    fseek(archivo,inicio_de_bloques,SEEK_SET);
    for(int y=0;y<final_bm_bloques;y++)
    {
        fwrite(&ocupado,sizeof(ocupado),1,archivo);
        inicio_de_bloques++;
        fseek(archivo,inicio_de_bloques,SEEK_SET);
    }

    //----- creacion del inodo para para la carpeta root
    inodo raiz;
    raiz.i_uid = 1;
    raiz.i_gid = 1;
    raiz.i_size = 0;
    strcpy(raiz.i_atime,FUN.obtenerFechaHora().c_str());
    strcpy(raiz.i_ctime,FUN.obtenerFechaHora().c_str());
    strcpy(raiz.i_mtime,FUN.obtenerFechaHora().c_str());
    for(int i=0; i<15; i++)
    {
        raiz.i_block[i] = -1;
    }
    raiz.i_type = 0;
    raiz.i_perm = 664;
    // primer apuntador hacia un bloque 
    raiz.i_block[0] = 0;

    // -----Bloque para la carpeta raiz
    bloque_carpetas blockCarpetaRaiz;
    // --------------------------------
    content contentCarpetaRaiz;
    // bloque actual
    strcpy(contentCarpetaRaiz.b_name,".");
    contentCarpetaRaiz.b_inodo = 0;
    blockCarpetaRaiz.b_content[0] = contentCarpetaRaiz;
    // padre
    strcpy(contentCarpetaRaiz.b_name,"..");
    contentCarpetaRaiz.b_inodo = 0;
    blockCarpetaRaiz.b_content[1] = contentCarpetaRaiz;
    // bloque para el archivo user.txt
    strcpy(contentCarpetaRaiz.b_name,"user.txt");
    contentCarpetaRaiz.b_inodo = 1;
    blockCarpetaRaiz.b_content[2] = contentCarpetaRaiz;
    // bloque vacio
    strcpy(contentCarpetaRaiz.b_name,"");
    contentCarpetaRaiz.b_inodo = -1;
    blockCarpetaRaiz.b_content[3] = contentCarpetaRaiz;

    // cout<<blockCarpetaRaiz.b_content[2].b_inodo<<endl;
    // cout<<blockCarpetaRaiz.b_content[2].b_name<<endl;

    //----Escribo el InodoRaiz 0
    // cout<<"inicio del inodod raiz "<<inicioParaInodo<<endl;
    fseek(archivo,inicioParaInodo,SEEK_SET);
    fwrite(&raiz,sizeof(raiz),1,archivo); 
    fseek(archivo,inicioParaBMInodo,SEEK_SET);//bitmap
    fwrite(&bmOcupado,sizeof(bmOcupado),1,archivo);//bitmap
    // -------------------------------------------
    inicioParaBMInodo++;
    inicioParaInodo = inicioParaInodo + sizeof(inodo);  
    // cout<<"incio para el otro indo "<<inicioParaInodo<<endl;     

    //----Escribo el bloque 0
    // cout<<"inicio del bloque 0 raiz "<<inicioParaBloque<<endl;
    fseek(archivo,inicioParaBloque,SEEK_SET);
    fwrite(&blockCarpetaRaiz,sizeof(blockCarpetaRaiz),1,archivo);
    fseek(archivo,inicioParaBMBloque,SEEK_SET);//bitmap
    fwrite(&bmOcupado,sizeof(bmOcupado),1,archivo);//bitmap
    inicioParaBloque = inicioParaBloque + sizeof(bloque_archivos);   
    inicioParaBMBloque++;  


    //------INODO PARA USERT.TXT
    char datosarchivo[]="1,G,root\n1,U,root,root,123\n";//cadena a quemar en el archivo
    inodo inodoUsers;
    inodoUsers.i_uid = 1;
    inodoUsers.i_gid = 1;
    inodoUsers.i_size = sizeof(datosarchivo);
    strcpy(inodoUsers.i_atime,FUN.obtenerFechaHora().c_str());
    strcpy(inodoUsers.i_ctime,FUN.obtenerFechaHora().c_str());
    strcpy(inodoUsers.i_mtime,FUN.obtenerFechaHora().c_str());
    for(int i=0; i<15; i++)
    {
        inodoUsers.i_block[i] = -1;
    }
    inodoUsers.i_type = 1;
    inodoUsers.i_perm = 664;
    //apuntador
    inodoUsers.i_block[0] = 1;

    //----Creacion del bloque 1
    bloque_archivos archivoUser;
    strcpy(archivoUser.b_content,datosarchivo);

    // //------Escritura del Inodouser 1
    fseek(archivo,inicioParaInodo,SEEK_SET);
    fwrite(&inodoUsers,sizeof(inodoUsers),1,archivo);
    // //bitmap inodo
    fseek(archivo,inicioParaBMInodo,SEEK_SET);
    fwrite(&bmOcupado,sizeof(bmOcupado),1,archivo);

    //----Escritura del Bloque
    fseek(archivo,inicioParaBloque,SEEK_SET);
    fwrite(&archivoUser,sizeof(archivoUser),1,archivo);
    //bitmap bloque
    fseek(archivo,inicioParaBMBloque,SEEK_SET);
    fwrite(&bmOcupado,sizeof(bmOcupado),1,archivo);

    // cierre del archivo
    fclose(archivo);
}




// funcion para calcular la cantidad de inodos
void mkfs::calcularInodos(vector<montajeDisco>&listado,string identificador,int journalin)
{
    // variables a=sizeDisco, b=sizeof(S.P), c=sizeof(J), d=sizeof(Inodo), e=sizeof(Block)
    string ruta = FUN.busquedaPathParticion(listado,identificador);
    int part_star = FUN.busquedaStarParticion(listado,identificador);
    this->inicioParticion = part_star;
    int a = FUN.busquedaSizeParticion(listado,identificador);
    this->tamanioParticion = a;
    int b = sizeof(superBloque);
    int c = journalin;
    int d = sizeof(inodo);
    int e = sizeof(bloque_archivos);
    cout<<"Ubicacion de la particion: "<<ruta<<endl;
    cout<<"Inicio de la particion: "<<part_star<<endl;
    cout<<"Tamanio del disco: "<<a<<endl;
    cout<<"Tamanio del S.P: "<<b<<endl;
    cout<<"Tamanio del Journal: "<<c<<endl;
    cout<<"Tamanio del Inodo: "<<d<<endl;
    cout<<"Tamanio del block: "<<e<<endl;   
    int cantidadInodos=(a-b)/(4+c+d+3*e);
    this->totalInodos = cantidadInodos;
    this->totalJournals = cantidadInodos;
    this->tamanioJournal = this->totalInodos * sizeof(journal);
    int cantidadBloques = 3*cantidadInodos;
    this->totalBloques = cantidadBloques;
    cout<<"Cantidad de Inodos: "<<cantidadInodos<<endl;
    cout<<"Cantidad de Bloques: "<<cantidadBloques<<endl;
}