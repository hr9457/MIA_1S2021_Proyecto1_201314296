#include "mkdisk.h"

/*metodo constructor inutilizable en esta clase*/
mkdisk::mkdisk()
{
}

/*metodo para crear un disco*/
void mkdisk::crearDisco(string parametros[])
{
    //empty() 1 vacio y 0 no vacio
    //cambiar el or a un and
    if(parametros[0].empty() != 1 && parametros[1].empty() != 1 )
    {
        vector<string> rutas;
        this->size = atoi(parametros[0].c_str());
        this->rutaArchivo = parametros[1];

        /*verificacion del atributo para el cambio del peso en el archivo*/
        if(parametros[2].empty() != true){this->pesoArchivo=FUN.aMinuscula(parametros[2]);}

        /*verificacion del artribuo fit para el cambio de ajuste*/
        if(parametros[3].empty() != true){cambioAjuste(parametros[3]);}

        /*verficacion de comillas en ruta*/
        if(rutaArchivo[0] == '\"')
        {
            //la ruta path viene entre comillas
            cout<<"-->la ruta viene entre comillas\n";
            this->rutaArchivo=eliminacionComillas(rutaArchivo);
            rutas = split(rutaArchivo,separador);  
            nombreDisco = rutas[rutas.size()-1];
            directorioDisco(rutas);
            crearFichero(this->rutaArchivo);//metodo para crear archivo binario
            cout<<"-->Creacion del disco: "<<nombreDisco<<" En la ubicacion: "<<rutaArchivo<<"\n"; 
        }
        else
        {
            //ruta path viene sin comillas
            rutas = split(rutaArchivo,separador);  
            nombreDisco = rutas[rutas.size()-1];//nombre del archivo
            directorioDisco(rutas);//ruta para el archivo
            crearFichero(this->rutaArchivo);//metodo para crear archivo binario
            cout<<"-->Creacion del disco: "<<nombreDisco<<" En la ubicacion: "<<rutaArchivo<<"\n";  
        }            
    }
    else
    {
        cout<<"-->parametro obligatorio faltantes: size,path\n";
    }
}

/*metodo para hacer split en la cadena de la ruta*/
vector<string> mkdisk::split(string str, char pattern)
{
    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> resultados;
    while(posFound >= 0){
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        resultados.push_back(splitted);
    }
    return resultados;
}

/*metodo para creacion y verficacion de carpetas*/
void mkdisk::crearCarpeta(string nombreCarpeta)
{
    if(mkdir(nombreCarpeta.c_str(),07777)==-1)
    {
        cout<<"-->Carpeta Existente\n";
    }
}

/*devuelve el directorio de la carpeta*/
string mkdisk::directorioDisco(vector<string> rutas)
{
    string rutaCreacion;
    int i = 0;
    for(i; i < rutas.size()-1; i++)
    {
        rutaCreacion += rutas[i];
        if(i+1<rutas.size()-1)
        {
            rutaCreacion += "/";
        }
    }
    //cout<<rutaCreacion<<"\n";
    crearCarpeta(rutaCreacion);
    return rutaCreacion;
}

string mkdisk::eliminacionComillas(string ruta)
{
    vector<string> resultado;
    string rutaSinComillas;
    resultado = split(ruta,'\"');
    rutaSinComillas = resultado[1];
    //cout << resultado[1] << endl;
    return rutaSinComillas;
}


/*metodo para la creacion del archivo binario*/
void mkdisk::crearFichero(string rutaArchivo)
{
    int tamanioArchivo;
    FILE *archivo;
    archivo = fopen(rutaArchivo.c_str(),"wb");
    if(archivo==NULL)
        exit(1);
    
    //char para llenado del archivo
    char buffer = '\0';

    // comparacion para el peso del archivo binario
    if(pesoArchivo == "k")
    {    
        int pesoKilobytes = this->size*1024;  
        tamanioArchivo = pesoKilobytes;      
        //for para el llenado de 0 en el archivo
        for(int i=0;i<pesoKilobytes;i++)
            fwrite(&buffer,1,1,archivo);
        //cierre del archivo
        fclose(archivo);
    }
    else
    {
        int pesoMegabytes = this->size*1024*1024;
        tamanioArchivo = pesoMegabytes;
        //for para el llenado de 0 en el archivo
        for(int i=0;i<pesoMegabytes;i++)
            fwrite(&buffer,1,1,archivo); 
        //cierre del archivo
        fclose(archivo);
    }

    //creacion de un MBR para el disco
    crearMBR(archivo,tamanioArchivo);
}


/*metodo para crear un mbr en el disco creado*/
void mkdisk::crearMBR(FILE *archivo,int tamanioArchivo)
{
    // crecion de fecha y hora para guardar en el MBR
    string horaFecha;
    time_t rawtime;
    struct tm * timeinfo;
    char bufferInfo[16];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime(bufferInfo,sizeof(bufferInfo),"%d/%m/%y %H:%M",timeinfo);

    //llenado de datos en la estructura del mbr
    mbr MBR;
    MBR.mbr_tamanio = tamanioArchivo;
    strcpy ( MBR.mbr_fecha_creacion, bufferInfo);
    MBR.mbr_disk_signature = (rand() % 100);
    MBR.mbr_fit = this->fit;
    //agreagacion los estados para las particiones del disco
    //quemando disco de prueba para el fdisk
    
    // partition PA;
    // PA.part_status = '0';
    // PA.part_type   = '-';
    // PA.part_fit    = '-';
    // PA.part_star   = -1 ;
    // PA.part_size   = -1 ;

    // partition PARTICION;
    // PARTICION.part_status = '1';
    // PARTICION.part_type   = 'p';
    // PARTICION.part_fit    = 'f';
    // PARTICION.part_star   = 291 ;
    // PARTICION.part_size   = 100 ;
    // strcpy(PARTICION.part_name,"P3");

    // partition PARTICION2;
    // PARTICION2.part_status = '1';
    // PARTICION2.part_type   = 'p';
    // PARTICION2.part_fit    = 'f';
    // PARTICION2.part_star   = 391 ;
    // PARTICION2.part_size   = 200 ;
    // strcpy(PARTICION2.part_name,"P2");

    // //insertando el disco de prueba las particiones
    // MBR.mbr_partitions[0] = PA;
    // MBR.mbr_partitions[1] = PARTICION;
    // MBR.mbr_partitions[2] = PARTICION2;
    // MBR.mbr_partitions[3] = PA;
    
    
    partition PARTICION;
    PARTICION.part_status = '0';
    PARTICION.part_type   = '-';
    PARTICION.part_fit    = '-';
    PARTICION.part_star   = -1 ;
    PARTICION.part_size   = -1 ;
    strcpy(PARTICION.part_name,"0");
    //llenado todos las particiones con los mismos parametros
    for(int i=0;i<4;i++)
    {
        MBR.mbr_partitions[i] = PARTICION;
    }
    
    //agregacion del MBR al archivo binario
    archivo = fopen(rutaArchivo.c_str(),"rb+");
    if(archivo!=NULL)
    {
        fseek(archivo,0,SEEK_SET);
        fwrite(&MBR,sizeof(MBR),1,archivo);
        fclose(archivo);
        cout<<"--->Creacion de MBR al dico: "<<nombreDisco<<endl;
    }
    else{ cout<<"--->MBR no creado imposible de acceder al disco: "<<nombreDisco<<endl; }
}


/*metodo para para recalcular el el fit del disco*/
void mkdisk::cambioAjuste(string tipoAjuste)
{
    //mejor ajuste, primer ajuste, peor ajuste
    if(tipoAjuste == "bf" || tipoAjuste == "BF")
    {this->fit = 'b';}
    else if(tipoAjuste == "ff" || tipoAjuste == "FF")
    {this->fit = 'f';}
    else if(tipoAjuste == "wf" || tipoAjuste == "WF")
    {this->fit = 'w';}
    else
    {cout<<"Tipo de ajuste no se reconoce se asignado el FB"<<endl;}
}
