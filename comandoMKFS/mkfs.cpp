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
            formatearParticion(listado,this->identificador);
            //selecionarSistemaArchivos();
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
    // cierre del dico con los cambios
    fclose(archivo);
}


// funcion para selecionar que sistema de archivos se instalar en la particion
void mkfs::selecionarSistemaArchivos()
{
    if(this->tipoSistema == "2fs")
    {
        cout<<"-->se ha instalado el sistemas de archivos ext2"<<endl;
    }
    else if(this->tipoSistema == "3fs")
    {
        cout<<"-->se ha instalado el sistema de archivos ext3"<<endl;
    }    
}


// funcion pra instalar sistema ext2
void mkfs::sistemaExt2()
{}


// funcion para instalar sistema ext3
void mkfs::sistemaExt3()
{}


// funcion para calcular la cantidad de inodos
void mkfs::calcularInodos(vector<montajeDisco>&listado,string identificador)
{
    // variables a=sizeDisco, b=sizeof(S.P), c=sizeof(J), d=sizeof(Inodo), e=sizeof(Block)
    string ruta = FUN.busquedaPathParticion(listado,identificador);
    int part_star = FUN.busquedaStarParticion(listado,identificador);
    int a = FUN.busquedaSizeParticion(listado,identificador);
    int b = sizeof(superBloque);
    int c = sizeof(journal);
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
    int cantidadBloques = 3*cantidadInodos;
    cout<<"Cantidad de Inodos: "<<cantidadInodos<<endl;
    cout<<"Cantidad de Bloques: "<<cantidadBloques<<endl;
}