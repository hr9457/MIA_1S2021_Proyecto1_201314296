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
        // verificacion de comillas en la ruta
        this->rutaArchivo = verificacionComillas(this->rutaArchivo);
        // verificacion de la existencia del disco
        if(verificacionDisco(this->rutaArchivo))
        {
            // verificacion comando add y delete no vengan juntos
            if(addDelete(parametros))
            {
                // si viene un size o un add entre los parametros
                if(parametros[1].empty()!=true || parametros[3].empty()!=true)
                {
                    // verificacion que los parametros size y add no vengan juntos
                    if(sizeAdd(parametros))
                    {  
                        // // si viene size o add
                        crearAgregarParticion(parametros);
                    }
                }
                // si viene el parametro delete entre los parametros
                else if(parametros[2].empty()!=true)
                {
                    eliminarParticion(parametros);
                }
                else 
                {
                    cout<<"-->Parametros faltantes"<<endl;
                }                                
            }            
        }        
    }
    else
    { cout<<"-->Parametros Obligatorios faltantes: path"<<endl;}
}


string fdisk::verificacionComillas(string rutaArchivo)
{
    if(rutaArchivo[0] == '\"')
    {
        return FUN.eliminacionComillas(rutaArchivo);
    }
    else
    {
        return rutaArchivo;
    }
}

// funcion para la verificacion de la existencia del archivo
bool fdisk::verificacionDisco(string rutaArchivo)
{
    if(FUN.is_file(rutaArchivo))
    {return true;}
    else
    {cout<<"-->No es posible acceder al disco"<<endl;return false;}
}


// metodo para la verificacion de que los paremetros add y delete no puedan venir juntos
bool fdisk::addDelete(string parametros[])
{
    // parametro add en posicion 1, parametro delete en posicion 2
    if(parametros[1].empty() != true && parametros[2].empty() != true)
    {   cout<<"-->Erro: parametros add y delete incompatibles"<<endl; return false;  }
    else
    {   return true;  }
}

// metodo para la verificacion de la incompatibilidad de los atributos size y add
bool fdisk::sizeAdd(string parametros[])
{
    // parametro add en posicion 1, parametro size en posicion 3
    if(parametros[1].empty() != true && parametros[3].empty() != true)
    { cout<<"-->parametros size y add incompatibles"<<endl; return false; }
    else
    { return true; } 
}



// fucion para actulizar los parametros opcionales para la creacion de un disco
void fdisk::parametrosOPcrear(string parametros[])
{
    // type tipo particion posicion5, f tipo de ajuste posicion 6, u tamanio disco posicion 7
    // tipo de particion a crear
    if(parametros[5].empty() != true)
    {
        this->type = FUN.aMinuscula(parametros[5]);
        if(this->type == "p"){this->tipoParticion = 'p';}
        else if(this->type == "e"){this->tipoParticion = 'e';}
        else if(this->type == "l"){this->tipoParticion = 'l';}
    }
    // para actualizacion del tipo de ajuste
    if(parametros[6].empty() != true)
    {
        this->f = FUN.aMinuscula(parametros[6]);
        if(this->f == "bf"){this->fitParticion='b';}
        else if(this->f == "ff"){this->fitParticion='f';}
        else if(this->f == "wf"){this->fitParticion='w';}        
    }
    // actualizacion el tamanio particion
    if(parametros[7].empty() != true)
    {
        this->u = FUN.aMinuscula(parametros[7]);
        if(this->u == "b"){this->sizeParticion = this->sizeParticion;}
        else if(this->u == "k"){this->sizeParticion = this->sizeParticion * 1024;}
        else if(this->u == "m"){this->sizeParticion = this->sizeParticion * 1024 * 1024;}
    }
    else
    {
        this->u="k";
        this->sizeParticion = this->sizeParticion * 1024;
    }
}

// metodo para verificacion si se creara una particion o agreagar a una particion espacio
void fdisk::crearAgregarParticion(string parametros[])
{
    // parametro add en posicion 1, parametro size en posicion 3, name posicion 4
    if(parametros[3].empty() != true && parametros[4].empty() != true)
    {
        // parametro name obligatorio
        this->sizeParticion = atoi(parametros[3].c_str());
        this->name = verificacionComillas(parametros[4]);
        parametrosOPcrear(parametros);
        crearParticion(this->sizeParticion);     
    }
    else
    {
        cout<<"-->parametros obligatorios faltantes: size,add,name"<<endl;
    }
}


// buscar espacios libres dentro del disco
void fdisk::encontrarEspaciosLibres()
{
    int tamanioDisco;
    FILE *archivo;
    // apertura del disco para lectura y actualizacion rb+
    archivo = fopen(rutaArchivo.c_str(),"rb+");    
    if(archivo==NULL)
        exit(1);

    // lectura del MBR en el disco
    mbr MBR;
    fseek(archivo,0,SEEK_SET); // inicio del archivo
    fread(&MBR,sizeof(mbr),1,archivo);
    // cierre del dico con los cambios
    fclose(archivo);

    /* 
    copia de las particiones dentro del disco para saber su 
    orden segun su poscion dentro del disco 
    status = 0 disco no utilizado
    status = 1 disco utilizado
    */    
    tamanioDisco = MBR.mbr_tamanio;// tamanio del disco que estamos leendo
    ajuste = MBR.mbr_fit;
    // ordenar el listado de las particiones segun supocicion part_star
    // impresion de datos del disco que estamos leendo 
    cout<<"---Datos del Disco---"<<endl;
    for(int i= 0;i<4;i++)
    {
        cout<<"---Particion No."<<i<<endl;
        cout<<"  *"<<MBR.mbr_partitions[i].part_status<<endl; 
        cout<<"  *"<<MBR.mbr_partitions[i].part_star<<endl;
        cout<<"  *"<<MBR.mbr_partitions[i].part_size<<endl;
        cout<<"  *"<<MBR.mbr_partitions[i].part_name<<endl; 
    }
    // busqueda de las particiones utilizadas  
    for(int i=0;i<4;i++)
    {
        partition2 particion;
        if(MBR.mbr_partitions[i].part_status == '1')
        {
            // contadores para saber cuantas particiones hay de cada tipo
            if(MBR.mbr_partitions[i].part_type == 'p'){contadorPrimarias++;totalParticiones++;}
            else if(MBR.mbr_partitions[i].part_type == 'e'){contadorExtendidas++;totalParticiones++;}
            else if(MBR.mbr_partitions[i].part_type == 'l'){contadorLogicas++;}
            //
            particion.part_star = MBR.mbr_partitions[i].part_star;
            particion.part_size = MBR.mbr_partitions[i].part_size;
            copia.push_back(particion);
        }        
    }
    //cout<<"===>Fin de la busquda de particiones utilizadas"<<endl;

    // ordenamiento de la copia con las particiones llenas en el disco
    // ordenamiento burbuja
    if(copia.empty()!=true){
        for(int i=0;i<copia.size()-1;i++)
        {
            for(int j=i+1;j<copia.size();j++)
            {
                if(copia[i].part_star > copia[j].part_star)
                {
                    partition2 aux = copia[i];
                    copia[i] = copia[j];
                    copia[j] = aux;
                }
            }
        }
    }
    //cout<<"===>fin de ordenamiento de particiones ocupadas"<<endl;
    
    // crecion de una lista con los espacios en blanco dentro del disco
    int inicio = sizeof(MBR);
    int inicioParticion;
    int sizeDisco;
    int espacioLibre;
    int inicioAnterior;
    for(int disco=0; disco<copia.size(); disco++)
    {
        inicioParticion = copia[disco].part_star;
        espacioLibre = inicioParticion - inicio;
        inicioAnterior = inicio;
        inicio = inicioParticion + copia[disco].part_size;
        if(espacioLibre>0)
        {
            //guardamos los epacios en blanco
            blackPartition particionLibre;
            particionLibre.part_star = inicioAnterior;
            particionLibre.part_size = espacioLibre;
            particionesLibres.push_back(particionLibre); 
        }        
    }    
    //cout<<"-->fin del la creacion de la lista con los espacios en blanco"<<endl;

    // condicion final si llego al final del disco
    if(inicio < MBR.mbr_tamanio)
    {
        espacioLibre = MBR.mbr_tamanio - inicio;
        blackPartition finalLibre;
        finalLibre.part_star = inicio;
        finalLibre.part_size = espacioLibre;
        particionesLibres.push_back(finalLibre);
    }
    
    // impresion de los espacios en blanco dentro del disco
    //impresionParticionesLibres();
}



// funcion para ordenar los espacios en blanco de una forma mayor a menor
void fdisk::ordenDescedente()
{
    // ordenamiento burbuja al reves
    // ordenamiento de la copia con las particiones llenas en el disco
    // ordenamiento burbuja
    for(int i=0;i<particionesLibres.size()-1;i++)
    {
        for(int j=i+1;j<particionesLibres.size();j++)
        {
            if(particionesLibres[i].part_size < particionesLibres[j].part_size)
            {
                blackPartition aux = particionesLibres[i];
                particionesLibres[i] = particionesLibres[j];
                particionesLibres[j] = aux;
            }
        }
    }
}

// ordenar espacios en blanco segun el tipo de ajuste
void fdisk::ordenarEspaciosLibres()
{
    /*
    ordenamiento de los espacios libres
    mejor ajuste -   ascendente
    primer ajuste -  no se ordena 
    peor ajuste -    descedente
    */
   if(ajuste == 'f')
   {
       // primer ajuste no se ordena la lista busca el primer espacio
   }
   else if(ajuste == 'b')
   {
       // mejor ajuste
   }   
   else if(ajuste == 'w')
   {
       // peor ajuste
       ordenDescedente();
   }
}




// funcion para buscar dentro de las particiones la que mejor se adecue 
void fdisk::buscarDentroParticion(int sizeParticion)
{
    // busqueda en las particiones ya ordenadas segun el fit del disco
    int inicio = 0;
    for(inicio; inicio < particionesLibres.size(); inicio++)
    {
        if(sizeParticion <= particionesLibres[inicio].part_size )
        {
            // escribir la nueva particion - verico el tipo de particion a crear exista
            if(tipoParticion=='p'||tipoParticion=='e'||tipoParticion=='l')
            {
                // verifico la cantidada de particiones existentes
                if(totalParticiones<4)
                {
                    // creacion de solo una particon extendida dentro del disco
                    if(tipoParticion=='e' && contadorExtendidas<1)
                    {
                        // creacion particiones primarias
                        int star = particionesLibres[inicio].part_star;
                        int sizeEncontrado = this->sizeParticion;//cambio
                        cout<<"Creacion de una particion tipo: "<<tipoParticion<<endl;
                        cout<<"particion de tamanio: "<<this->sizeParticion<<""<<this->u<<endl;
                        cout<<"Con fit de: "<<this->fitParticion<<endl;
                        cout<<"El nombre de: "<<this->name<<endl;
                        cout<<"EN PARTICION No."<<inicio<<endl;
                        agregarActualizarMBR('1',tipoParticion,this->fitParticion,star,sizeEncontrado,this->name);
                        break;
                    }
                    else if(tipoParticion=='l' && contadorExtendidas>1)
                    {
                        // creacion de partion logica si solo hay extendidas
                        cout<<"Creacion de una particion tipo: "<<tipoParticion<<endl;
                        cout<<"particion de tamanio: "<<this->sizeParticion<<""<<this->u<<endl;
                        cout<<"Con fit de: "<<this->fitParticion<<endl;
                        cout<<"El nombre de: "<<this->name<<endl;
                        cout<<"EN PARTICION No."<<inicio<<endl;
                        break;
                    }
                    else if(tipoParticion=='p')
                    {
                        // creacion particiones primarias
                        int star = particionesLibres[inicio].part_star;
                        int sizeEncontrado = this->sizeParticion;//cambio
                        cout<<"-->Creacion de una particion tipo: "<<tipoParticion<<endl;
                        cout<<"-->particion de tamanio ingresada: "<<this->sizeParticion<<""<<this->u<<endl;
                        cout<<"-->Con fit de: "<<this->fitParticion<<endl;
                        cout<<"-->El nombre de: "<<this->name<<endl;
                        cout<<"-->que inicia en: "<<star<<endl;
                        cout<<"-->que tiene tmanio de: "<<sizeEncontrado<<endl;
                        cout<<"-->PARTICION  CREADA"<<endl;
                        agregarActualizarMBR('1',tipoParticion,this->fitParticion,star,sizeEncontrado,this->name);
                        //cout<<"-->MBR  ACTUALIZADO"<<endl;
                        // fdisk -path=/home/hector/prueba/discoFF2.dk -size=200 -u=b -type=p -f=ff -name=hector
                        break;
                    }
                }
                else
                {cout<<"-->El Disco ha llegado a su numero limite de particiones"<<endl;break;}                
            }
            else
            {
                cout<<"-->Tipo de particion no se puede crear"<<endl;
                break;
            }            
        }
    }
    inicio++;
    if(inicio > particionesLibres.size())
    {
        cout<<"-->No Hay espacio suficiente dentro del disco para la particion de: "<<sizeParticion<<endl;
    }
}



// metodo para crear particion en el disco
void fdisk::crearParticion(int sizeParticion)
{
    //  busco los espacios libres dentro del disco
    encontrarEspaciosLibres();
    //  ordeno los espacios en blanco segun el fit del disco
    ordenarEspaciosLibres(); 
    // busco donde pueda caber la particion que el usuario quiera ingresar
    buscarDentroParticion(this->sizeParticion);
    // 
}


//funcion para leer y actualizar el mbr del disco
void fdisk::agregarActualizarMBR(char status,char type,char fit,int star,int size,string name)
{
    // se crea una nueva particon con los datos nuevos
    partition nuevaParticion;
    nuevaParticion.part_status = status;
    nuevaParticion.part_type = type;
    nuevaParticion.part_fit = fit;
    nuevaParticion.part_star = star;
    nuevaParticion.part_size = size;
    strcpy(nuevaParticion.part_name,name.c_str());

    FILE *archivo;
    // apertura del disco para lectura y actualizacion rb+
    archivo = fopen(rutaArchivo.c_str(),"rb+");    
    if(archivo==NULL)
        exit(1);

    mbr MBR; // copia de los datos del mbr
    // posiciono al incio del archivo
    fseek(archivo,0,SEEK_SET); // inicio del archivo
    fread(&MBR,sizeof(mbr),1,archivo);// lee el mbr del disco

    //busco una particion basia donde insertar
    for(int particion=0; particion<4; particion++)
    {
        // busco una particion con estatus = 0
        if(MBR.mbr_partitions[particion].part_status=='0')
        {
            // introduccimos nueva particion en esta posicion
            MBR.mbr_partitions[particion] = nuevaParticion;
            break;
        }
    }
    // posiciono al inicio del archivo
    fseek(archivo,0,SEEK_SET); // inicio del archivo
    // re-escribo el nuevo mbr para actualizarlo
    fwrite(&MBR,sizeof(MBR),1,archivo);
    // cierre del dico con los cambios
    fclose(archivo);
    //cout<<"===>CIRRE DEL ARCHIVO"<<endl;
}


// funcion para imprimir la particiones libres dentro del disco
void fdisk::impresionParticionesLibres()
{
    // impresion de los espacios en blanco dentro del disco
    cout<<"******Espcios en Blanco*********"<<endl;
    for(int inicio = 0;inicio<particionesLibres.size();inicio++)
    {
        cout<<"Particion Libre No."<<inicio<<endl;
        cout<<"Espacio: "<<particionesLibres[inicio].part_size<<endl;
        cout<<"Inicio: "<<particionesLibres[inicio].part_star<<endl;
        cout<<endl;
        cout<<endl;
    }
}


// funcion para eleminar particiones dentro de un disco
void fdisk::eliminarParticion(string parametros[])
{
    // busqueda del parametro name = id,cadena en la particiones
    if(parametros[4].empty()!=true)
    {
        string tipoEliminacion = FUN.aMinuscula(parametros[2]);
        string nombreParticion = verificacionComillas(parametros[4]);
        if(tipoEliminacion == "fast" || tipoEliminacion == "full")
        {
            eliminarActulizarMBR(tipoEliminacion,nombreParticion); // actualizar el disco y MBR
        }
        else
        {
            cout<<"-->Falta de parametro en delete obligatorios: fat o full"<<endl;
        }
    }
    else
    {
        cout<<"-->parametros obligatorio para eliminar: name"<<endl;
    }
}



// funcion para elimar dentro del disco y actualizar el MBR del disco
void fdisk::eliminarActulizarMBR(string tipoEliminacion,string nombreParticion)
{
    mbr MBR;
    FILE *archivo;
    // apertura del disco para lectura y actualizacion rb+
    archivo = fopen(rutaArchivo.c_str(),"rb+");    
    if(archivo==NULL)
        exit(1);

    // posiciono al principio y leo el MBR del disco
    fseek(archivo,0,SEEK_SET); // inicio del archivo
    fread(&MBR,sizeof(mbr),1,archivo); // lectura del MBR   
    //busqueda de una particion el nombre que el usuario quiere eliminar
    for(int particion=0;particion<4;particion++)
    {
        string particionActual = FUN.vectorAstring(MBR.mbr_partitions[particion].part_name);
        if(strcmp(nombreParticion.c_str(),particionActual.c_str())==0)
        {
            // verificacion del tipo de eliminacion que se hara dentro del discos
            if(tipoEliminacion == "fast")
            {
                cout<<"-->Se realizao una elminacion dentro del disco tipo fast"<<endl;
                cout<<"-->de la particion: "<<nombreParticion<<endl;
                // actualizacion en el MBR del disco
                //actualizacon para el MBR del disco 
                MBR.mbr_partitions[particion].part_status = '0';
                MBR.mbr_partitions[particion].part_type = '-';
                MBR.mbr_partitions[particion].part_fit = '-';
                MBR.mbr_partitions[particion].part_star = -1;
                MBR.mbr_partitions[particion].part_size = -1;
                strcpy(MBR.mbr_partitions[particion].part_name,"0");
                //actualizo el MBR del disco
                fseek(archivo,0,SEEK_SET); // inicio del archivo
                fwrite(&MBR,sizeof(MBR),1,archivo); //escribo y actualizo
                break;
            }
            // eliminacion full - llenado con 0 en la particion del disco
            else if(tipoEliminacion == "full")
            {
                cout<<"-->Se realizao una elminacion dentro del disco tipo full"<<endl;
                cout<<"-->de la particion: "<<nombreParticion<<endl;                          
                // llenando de caracters la particion eliminada por full
                int inicioParticion = MBR.mbr_partitions[particion].part_star;
                int tamanioParticion = MBR.mbr_partitions[particion].part_size;
                char caracterVaciado = '\0';
                // llenando de caracters esta particion a elminacion full
                for(int i=0; i<tamanioParticion; i++)
                {     
                    fseek(archivo,inicioParticion,SEEK_SET);               
                    fwrite(&caracterVaciado,sizeof(caracterVaciado),1,archivo); // escribo en esa posicion                     
                    inicioParticion++;
                }
                //actualizacon para el MBR del disco 
                MBR.mbr_partitions[particion].part_status = '0';
                MBR.mbr_partitions[particion].part_type = '-';
                MBR.mbr_partitions[particion].part_fit = '-';
                MBR.mbr_partitions[particion].part_star = -1;
                MBR.mbr_partitions[particion].part_size = -1;
                strcpy(MBR.mbr_partitions[particion].part_name,"0");
                //actualizo el MBR del disco
                fseek(archivo,0,SEEK_SET); // inicio del archivo
                fwrite(&MBR,sizeof(MBR),1,archivo); //escribo y actualizo
                break;
            }
        }
    }
    // cierre del archivo
    fclose(archivo);
    cout<<"-->Eliminacion y Actualicion Exitosa"<<endl;
}
