#include "mount.h"

mount::mount()
{
}

// metodo para montar un disco
void mount::montarDisco(string parametros[],vector<montajeDisco> &listadoDiscos,int &numeroDisco)
{
    // path posicion 0, name posicion 1
    if(parametros[0].empty()!=true && parametros[1].empty()!=true)
    {
        // guardo la ruta que venga en el path
        this->rutaArchivo = parametros[0];
        // busco si viene comillas en ruta
        this->rutaArchivo = verificacionComillas(this->rutaArchivo);
        // verificacion de la existencia del disco
        if(verificacionDisco(this->rutaArchivo))
        {
            addLista(this->rutaArchivo,numeroDisco,listadoDiscos,parametros[1]);
            impresionListadoParticones(listadoDiscos);
            // busquedaParticionDisco(this->rutaArchivo);
        }
    }
    else
    {
        cout<<"-->Parametros obligatorios faltantes: path, name"<<endl;
    }
}


// verficacion de comillas en la ruta
string mount::verificacionComillas(string rutaArchivo)
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



// metodo para la verificacion de la existencia del disco
bool mount::verificacionDisco(string rutaArchivo)
{
    if(FUN.is_file(rutaArchivo))
    {return true;}
    else
    {cout<<"-->No es posible acceder al disco"<<endl;return false;}
}


// metodo para crear y agregar a lista
void mount::addLista(string ruta,int &numeroDisco,vector<montajeDisco>&listado,string nameDisco)
{
    // buscar la existencia de una particion en el disco
    if(searchParticionDisco(ruta,nameDisco))
    {
        int busquedaDisco = 0; // si se encuntr una disco ya montado
        int discoNuevo = 1; // valor para saber si el disco ya se encuentra montado en memoria
        montajeDisco particionMontada;
        int particionRepetida; // guardar la poscion del disco ya montado
        // busqueda de la existencia del disco en el listado
        for(particionRepetida=0;particionRepetida<listado.size();particionRepetida++)
        {
            if(listado[particionRepetida].rutaArchivo == ruta)
            {
                busquedaDisco=1;
                particionMontada = listado[particionRepetida];
                break;
            }
        }

        // verificacion de la existencia del disco
        if(busquedaDisco==1)
        {
            //cout<<"Disco ya Montado"<<endl;
            // si ya se encuentra hay que montar la particion
            if(!searchParticionMontada(listado,nameDisco))
            {
                for(int i=0;i<4;i++)
                {
                    //busco una particion libre
                    if(particionMontada.particiones[i].status == '0')
                    {
                        cout<<"particion libre encontrada"<<endl;
                        int numeroDiscoEncontrado = particionMontada.numeroDisco;
                        discoNuevo = 0; // saber si hay que buscar un letra nueva para el id
                        string identificadParticion = generarIdentificador(numeroDiscoEncontrado,discoNuevo,particionMontada);
                        listado[particionRepetida].particiones[i].status = '1';
                        listado[particionRepetida].particiones[i].sizeDisco = this->tamanioParticion;
                        strcpy(listado[particionRepetida].particiones[i].identificador,identificadParticion.c_str());
                        strcpy(listado[particionRepetida].particiones[i].nombreParticion,nameDisco.c_str());
                        cout<<"Particion montada"<<endl;
                        break;
                    }
                }
            }

        }
        else
        {

            montajeDisco nuevaParticion;
            strcpy(nuevaParticion.rutaArchivo,ruta.c_str());
            nuevaParticion.numeroDisco = numeroDisco;
            //
            partitionMontada datosParticion;
            strcpy(datosParticion.identificador,"-");
            strcpy(datosParticion.nombreParticion,"-");
            datosParticion.status = '0';
            datosParticion.sizeDisco = -1;
            //despojando espacios para montar particiones
            for(int i=0;i<4;i++)
            {nuevaParticion.particiones[i] = datosParticion;}

            
            //monto la particion  
            int espacioParticionesMontadas=0; 
            // buscar espacio para montar particion
            for(int i=0;i<4;i++)
            {
                if(nuevaParticion.particiones[i].status == '0')
                {
                    espacioParticionesMontadas=1;
                    string identificadorParticion = generarIdentificador(numeroDisco,discoNuevo,nuevaParticion);
                    nuevaParticion.particiones[i].status = '1';
                    nuevaParticion.particiones[i].sizeDisco = this->tamanioParticion;
                    strcpy(nuevaParticion.particiones[i].identificador,identificadorParticion.c_str());
                    strcpy(nuevaParticion.particiones[i].nombreParticion,nameDisco.c_str());
                    break;
                }
            } 
            // inserto particion
            listado.push_back(nuevaParticion);    
            numeroDisco++;  

        }

    }//fin de condicion de busqueda dentro del disco
    else
    {
        cout<<"-->La particion: "<<nameDisco<<" no exite en el disco"<<endl;
    }    
}


// metodo para generar un identificador
string mount::generarIdentificador(int &numeroDisco,int discoNuevo,montajeDisco &discoMontado)
{
    if(discoNuevo == 1)
    {
        this->identificador += this->carnet;
        this->identificador += to_string(numeroDisco);
        this->identificador += abc[0];        
        //cout<<"-->Identificador generado: "<<this->identificador<<endl;
        return identificador;
    }
    else if(discoNuevo == 0)
    {        
        int numeroLetra = 0;
        char letra = abc[numeroLetra];
        int numeroParticion=0;
        bool letraEncontrada=false;
        this->identificador += this->carnet;
        this->identificador += to_string(numeroDisco);
        //--------------------------------------------
        // guardo la letra par verificacion
        // recorrido de todoas la particiones
        for(int i=0;i<4;i++)
        {
            // solo particiones en uso
            if(discoMontado.particiones[i].status == '1')
            {
                string idParticion = discoMontado.particiones[i].identificador;
                int ultimaPosicion = idParticion.length()-1;
                char ultimaLetra = idParticion[ultimaPosicion];
                if(letra==ultimaLetra)
                {
                    numeroLetra++;
                    i=0; // reviso la siguiente letra del abc si tambien esta usada
                    letra = abc[numeroLetra]; // paso a la siguiente letra
                }
            }
        }
        this->identificador += letra;
        //cout<<"letra que se va usar en el siguiente id: "<<letra<<endl;
        return identificador;
    }    
}


// metodo para imprimir los discos montados actualmente
void mount::impresionListadoParticones(vector<montajeDisco>&listado)
{
    // recorrido de cada particion montada existente
    cout<<endl;
    cout<<"*****Listado de particiones montadas*****"<<endl;
    for(int i=0;i<listado.size();i++)
    {
       montajeDisco discoActual = listado[i]; 
       cout<<"-->Ubicacion: "<<discoActual.rutaArchivo<<endl;
       cout<<"-->No. Disco: "<<discoActual.numeroDisco<<endl;
       for(int i=0;i<4;i++)
       {
           if(discoActual.particiones[i].status != '0')
           {
               cout<<"  -->STATUS: "<<discoActual.particiones[i].status<<endl;
               cout<<"  -->ID: "<<discoActual.particiones[i].identificador<<endl;
               cout<<"  -->NAME: "<<discoActual.particiones[i].nombreParticion<<endl<<endl;
           }
       }
    }
}



//  metodo para buscar la existencia de una particon dentro de un disco
bool mount::searchParticionDisco(string rutaDisco,string busquedaParticion)
{
    FILE *archivo;
    // apertura del disco para lectura y actualizacion rb+
    archivo = fopen(rutaArchivo.c_str(),"rb+");    
    if(archivo==NULL)
        exit(1);

    mbr MBR; // copia de los datos del mbr
    // posiciono al incio del archivo
    fseek(archivo,0,SEEK_SET); // inicio del archivo
    fread(&MBR,sizeof(mbr),1,archivo);// lee el mbr del disco
    fclose(archivo);
    // recorro en busqueda la particion existente
    for(int i=0;i<4;i++)
    {
        if(!strcmp( MBR.mbr_partitions[i].part_name,busquedaParticion.c_str()))
        {
            this->tamanioParticion = MBR.mbr_partitions[i].part_size;
            return true;
        }
    }
    return false;
}


// metodo para buscar una particion no este montada anterior mente
bool mount::searchParticionMontada(vector<montajeDisco>&listado,string nombreParticion)
{
    // recorro todos los elementos del listado principal
    for(int i=0;i<listado.size();i++)
    {
        // recorro particion por particion del disco para comprobar que no este motada 
        for(int j=0;j<4;j++)
        {
            if(listado[i].particiones[j].status == '1' && !strcmp(listado[i].particiones[j].identificador,nombreParticion.c_str()))
            {
                return true;
            }
            //cout<<"-->busqueda en la siguiente particion montada"<<endl;
        }
    }
    return false;
}