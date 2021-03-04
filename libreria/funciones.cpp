#include "funciones.h"

funciones::funciones()
{

}

/*metodo para hacer split en la cadena de la ruta*/
vector<string> funciones::split(string str, char pattern)
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

/*metodo para eliminar comillas de una ruta path*/
string funciones::eliminacionComillas(string ruta)
{
    vector<string> resultado;
    string rutaSinComillas;
    resultado = split(ruta,'\"');
    rutaSinComillas = resultado[1];
    //cout << resultado[1] << endl;
    return rutaSinComillas;
}

/*metodo para saber si un archivo existe en el sistema operativo*/
bool funciones::is_file(string file)
{
    FILE *archivo;
    if(archivo = fopen(file.c_str(),"r"))
    {
        fclose(archivo);
        return true;
    }
    else
    {
        return false;
    }
}


/* metodo par convertir un string a mayusculas */
string funciones::aMayuscula(string cadena)
{
  for (int i = 0; i < cadena.length(); i++) cadena[i] = toupper(cadena[i]);
  return cadena;   
}


/* metodo para convertir un string a minusculas */
string funciones::aMinuscula(string cadena) 
{
  for (int i = 0; i < cadena.length(); i++) 
  {cadena[i] = tolower(cadena[i]);}
  return cadena;
}

/* metodo para limpiar cualquier vector */
void funciones::limpiarVector(string limpieza[])
{
    int tamanio = sizeof(limpieza)/sizeof(limpieza[0]);
    cout<<"El tamanio del vector a limpiar es de: "<<tamanio<<endl;
}


// metodo para buscar un elemento dentro de un array
bool funciones::buscarElemento(string arreglo[],string elemento)
{
    string nombreActual;
    bool resultado;
    for(int i = 0; i < sizeof(arreglo)/sizeof(arreglo[0]); i++ )
    {
        nombreActual = arreglo[i];
        if(nombreActual == elemento)
        {
            resultado = true;
            break;
        }
    }
    // retorno a la busqueda
    if(resultado)
    {return true;}
    else
    {return false;}
}


// metodo para convertir un arreglo de char a un string 
string funciones::vectorAstring(char arreglo[])
{
    int limite = sizeof(arreglo)/sizeof(arreglo[0]);
    string palabra = "";
    for(int i=0;i<limite;i++)
    {
        palabra += arreglo[i];
    }
    return palabra;
}


// metodo para buscar un elemento dentro de un vector
bool funciones::elementoEnVector(vector<string>arreglo,string busqueda)
{
    for(int i=0; i<arreglo.size(); i++)
    {
        // strcmp compara dos string
        if(strcmp(arreglo[i].c_str(),busqueda.c_str())==0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}



// metodo para obtener la hora actual del sistema
string funciones::obtenerFechaHora()
{
    // crecion de fecha y hora para guardar en el MBR
    string horaFecha;
    time_t rawtime;
    struct tm * timeinfo;
    char bufferInfo[16];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime(bufferInfo,sizeof(bufferInfo),"%d/%m/%y %H:%M",timeinfo);
    //convierto  todo el char a un string
    for(int i=0;i<16;i++)
    {
        horaFecha += bufferInfo[i];
    }    
    return horaFecha;
}