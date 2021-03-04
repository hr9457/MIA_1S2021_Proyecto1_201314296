#include "unmount.h"

unmount::unmount()
{   
}


// funcion para desmontar un disco
void unmount::desmontarDisco(vector<montajeDisco> &listado,string identificador)
{
    cout<<"-->Desmontando particion: "<<identificador<<endl;
    busquedaParticion(listado,identificador);
    impresionMontaje(listado);
}



// metodo para buscar la existencia de la particion en el listados de discos montados
bool unmount::busquedaParticion(vector<montajeDisco>&listado,string identificador)
{
    int particionesMontadas=0;
    //recorro todo el listado disco a disco para encontra el particon montada
    for(int i=0;i<listado.size();i++)
    {
        //recorro todas las particiones montadas posibles dentro de ese disco
        for(int particion=0;particion<4;particion++)
        {
            // comparacion del id con el id que esta buscando el usuario
            if(!strcmp(listado[i].particiones[particion].identificador,identificador.c_str()))
            {
                // cout<<"Particion: "<<identificador<<" listado Posicion: "<<i<<endl;
                listado[i].particiones[particion].status = '0';
                strcpy(listado[i].particiones[particion].identificador,"");
                strcpy(listado[i].particiones[particion].nombreParticion,"");
                cout<<"-->Particion Desmontada"<<endl;
                return true;
            }
        }
    }
    cout<<"-->La particion a Desmontar no se encuentra en el listado"<<endl;
    return false;
}


// funcion para imprimir todas los disco y particiones montadas
void unmount::impresionMontaje(vector<montajeDisco>&listado)
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


// funcin para contar si el disco aun tiene particiones montadas
void unmount::revisionParticionesDiscos(vector<montajeDisco>&listado)
{
    int particionesMontadas = 0;
    // recorrido de toda la lista con los discos montados
    for(int disco=0;disco<listado.size();disco++)
    {
        // recorro las particiones montadas por ese disco
        for(int particion=0;particion<4;particion++)
        {
            // busco las particiones que esten con el status 1
            if(listado[disco].particiones[particion].status == '1')
            {
                particionesMontadas++;
            }
        }
    }
    // fin del ciclo for
}

