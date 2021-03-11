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



// metodo para saber si la particion esta montada por el comando mount
bool funciones::busquedaParticion(vector<montajeDisco>&listado,string identificador)
{
    // verificacion de algun elmento en el disco
    // busqueda disco por disco
    for(int disco=0;disco<listado.size();disco++)
    {
        // busqueda particion por particion
        for(int particion=0;particion<4;particion++)
        {
            if(listado[disco].particiones[particion].status == '1' && strcmp(listado[disco].particiones[particion].nombreParticion,identificador.c_str()) )
            {
                return true;
            }
        }            
    }
    return false;
}


// metodo para saber el size de la particon montada por el comando mount
int funciones::busquedaSizeParticion(vector<montajeDisco>&listado,string identificador)
{
    int size=0;
    // verificacion de algun elmento en el disco
    // busqueda disco por disco
    for(int disco=0;disco<listado.size();disco++)
    {
        // busqueda particion por particion
        for(int particion=0;particion<4;particion++)
        {
            if(listado[disco].particiones[particion].status == '1' && strcmp(listado[disco].particiones[particion].nombreParticion,identificador.c_str()) )
            {
                size = listado[disco].particiones[particion].sizeDisco;
                return size;
            }
        }            
    }
    return size;
}


// metod para saber el inicio de una particion montada por el comando mount
int funciones::busquedaStarParticion(vector<montajeDisco>&listado,string identificador)
{
    int part_star=0;
    // verificacion de algun elmento en el disco
    // busqueda disco por disco
    for(int disco=0;disco<listado.size();disco++)
    {
        // busqueda particion por particion
        for(int particion=0;particion<4;particion++)
        {
            if(listado[disco].particiones[particion].status == '1' && strcmp(listado[disco].particiones[particion].nombreParticion,identificador.c_str()) )
            {
                part_star = listado[disco].particiones[particion].part_star;
                return part_star;
            }
        }            
    }
    return part_star;
}


// metodo para saber la ruta de un archiv de una particon montada por el comando mount
string funciones::busquedaPathParticion(vector<montajeDisco>&listado,string identificador)
{
    string path="";
    // verificacion de algun elmento en el disco
    // busqueda disco por disco
    for(int disco=0;disco<listado.size();disco++)
    {
        // busqueda particion por particion
        for(int particion=0;particion<4;particion++)
        {
            if(listado[disco].particiones[particion].status == '1' && strcmp(listado[disco].particiones[particion].nombreParticion,identificador.c_str()) )
            {
                path = listado[disco].rutaArchivo;
                return path;
            }
        }            
    }
    return path;
}




// metodo para convertir un cadena de caracteres tipo char[] = {""}
string funciones::convertirArreglochar(char contenido[])
{
    string palabra;
    char *informacion[]={};
    informacion[0] = contenido;
    palabra = informacion[0];
    return palabra;
}

// metodo para buscar la existencia de un usuario y su password esten correctos
bool funciones::buscarDentroVector(string listado,string palabra)
{
    string frase;
    // quitar los saltos de linea
    for(int i=0; i<listado.size(); i++)
    {
        if(listado[i] != '\n')
        {
            frase += listado[i];
        }
    }
    // cout<<frase<<endl;
    vector<string> fraseSeparada =  split(frase,',');
    //
    for(int elemento=0; elemento<fraseSeparada.size(); elemento++)
    {
        if(fraseSeparada[elemento] == palabra)
        {            
            return true;
        }
    }
    return false;
}




// metodo para del inodo que pertene al archivo y carpeta
int funciones::buscarInodoArchivoCarpeta(FILE *archivo,int part_star,char texto[])
{
    // lectura del superbloque del archivo que se manda
    superBloque SP;
    fseek(archivo,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,archivo);

    // ----lectura del primero inodo - inodo raiz
    inodo raiz;
    fseek(archivo,SP.s_inode_start,SEEK_SET);
    fread(&raiz,sizeof(raiz),1,archivo);

    // ----- variables para tener los inicios que me da el SP
    int inicio_Inodos = SP.s_inode_start;
    int inicio_Bloques = SP.s_block_start;
    int inicio_bm_Bloques = SP.s_bm_block_start;
    int inodoDelArchivoCarpeta = -1;
    bloque_carpetas bloque_busqueda;


    // ----- Algoritmo de busqueda
    for(int apuntador=0; apuntador<12; apuntador++)
    {
        // --- revision si los apuntadores estan o no
        if(raiz.i_block[apuntador] != -1)
        {
            // ---- numero de bloque en el inodo raiz
            int numero_bloque = raiz.i_block[apuntador];
            cout<<numero_bloque<<endl;
            if(numero_bloque > 0){inicio_Bloques = numero_bloque * sizeof(bloque_carpetas);}

            // --- posicion de inicio del bloque n
            fseek(archivo,inicio_Bloques,SEEK_SET);
            fread(&bloque_busqueda,sizeof(bloque_busqueda),1,archivo);

            // --- lectura de cada uno de los apuntadores del bloque
            for(int apuntadorBloque=0; apuntadorBloque<4; apuntadorBloque++)
            {
                if(bloque_busqueda.b_content[apuntadorBloque].b_inodo != -1)
                {
                    if(strcmp(bloque_busqueda.b_content[apuntadorBloque].b_name,texto)==0)
                    {
                        cout<<bloque_busqueda.b_content[apuntadorBloque].b_name<<"<--->";
                        cout<<bloque_busqueda.b_content[apuntadorBloque].b_inodo<<endl;
                        inodoDelArchivoCarpeta = bloque_busqueda.b_content[apuntadorBloque].b_inodo;
                        break;
                    }                    
                }
            }
            // --- reinicio 
            inicio_Bloques = SP.s_block_start;
        }
    }

    return inodoDelArchivoCarpeta;
}



// metodo para tener la lectura del archivo txt linea por linea
void funciones::lineaAlinea()
{

}


// funcion par concatenar todas las lineas de un archivo de texto y insertar en un bloque
string funciones::concatenarArchivoTexto(FILE *archivo,int part_star,inodo inodoTexto,char nuevo[],int longitudNuevo)
{
    string texto;

    // lectura del superbloque del archivo que se manda
    superBloque SP;
    fseek(archivo,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,archivo);

    // ----- inicios de los bloques
    int inicio_bloques = SP.s_block_start;
    // cout<<"inicio de los bloques: "<<inicio_bloques<<endl;

    // ---- recorrer cada espacio del apuntador directo para ver si tiene algo y concatenar
    for(int apuntador = 0; apuntador<12; apuntador++)
    {
        // cout<<inodoTexto.i_block[apuntador]<<endl;
        // ---- recorro cada apuntador del inodo del archivo de texto
        if(inodoTexto.i_block[apuntador] != -1)
        {
            cout<<"bloque examinado"<<endl;
            // --- numero del bloque del inodo del arhivo de texto
            int numero_bloque = inodoTexto.i_block[apuntador];
            int inicio_bloque_texto = inicio_bloques + ( numero_bloque * sizeof(bloque_archivos) );
            // cout<<"Numero de bloque de texto: "<<numero_bloque<<endl; 
            // cout<<"Poisicion del bloque de texto: "<<inicio_bloque_texto<<endl; 

            // ----- lectura y datos del bloque
            bloque_archivos bloque_texto;
            fseek(archivo,inicio_bloque_texto,SEEK_SET);
            fread(&bloque_texto,sizeof(bloque_texto),1,archivo);
            texto += bloque_texto.b_content;
            int longitudTexto = texto.size();            

            // --- revision en el bloque si aun hay espacio
            if(longitudTexto<63)
            {
                // --- cuantos caracters se perminten aun 
                int caracters_permitidos = 63 - longitudTexto;
                cout<<"Caracters permitod en el bloque son: "<<caracters_permitidos<<endl;
                // ------ colocar los caracteres
                if(caracters_permitidos>longitudNuevo)
                {
                    strcat(bloque_texto.b_content,nuevo);
                    cout<<bloque_texto.b_content<<endl;
                    // ---- actualizacion en el bloque 
                    fseek(archivo,inicio_bloque_texto,SEEK_SET);
                    fwrite(&bloque_texto,sizeof(bloque_texto),1,archivo);
                }
            }
        }
    }
    // ----- inserto en archio de texto lo que se quiere inisertar
    return texto;
}