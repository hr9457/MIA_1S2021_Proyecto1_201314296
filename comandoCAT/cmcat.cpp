#include "cmcat.h"

cmcat::cmcat()
{
}

void cmcat::leerArchivo(vector<usuarioConectado>&usuario,vector<montajeDisco>&discos,string parametro)
{
    if(verificacionUsuario(usuario))
    {
        if(verificacionParametro(parametro))
        {
            // parametro filen
            this->parmetro_filen = eliminarComillas(parametro);
            // buscquda y impresion del archivo txt
            buscarArchivoTXT(this->parmetro_filen,usuario,discos);
        }
        else
        {
            cout<<"----> parametro obligatorios faltantes: filen"<<endl;
        }
    }
    else
    {
        cout<<"----> No hay ninguna session activa"<<endl;
    }
    cout<<endl;
}


bool cmcat::verificacionUsuario(vector<usuarioConectado>&usuarios)
{
    if(usuarios.size()>0)
    {return true;}
    else
    {return false;}
}

bool cmcat::verificacionParametro(string parametro)
{
    if(parametro.empty()!=true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ---- eliminacion de comillas de una ruta
string cmcat::eliminarComillas(string palabra)
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


void cmcat::buscarArchivoTXT(string parametro,vector<usuarioConectado>&usuarios,vector<montajeDisco>&discos)
{
    string identificador = usuarios[0].identificador;
    string rutaArchivo = FUN.busquedaPathParticion(discos,identificador);
    int part_star = FUN.busquedaStarParticion(discos,identificador);


    // aperturar el archivo
    FILE *particion;
    particion = fopen(rutaArchivo.c_str(),"rb+");

    superBloque SP;
    fseek(particion,part_star,SEEK_SET);
    fread(&SP,sizeof(superBloque),1,particion);

    int inicio_inodos = SP.s_inode_start;
    int inicio_bloques = SP.s_block_start;
    int numero_inodo = 0;
    bool resultado_busqueda = true;
    
    vector<string> carpetas_separadas = FUN.split(parametro,'/');

    fseek(particion,inicio_inodos,SEEK_SET);

    for(int numero_carpeta=1; numero_carpeta<carpetas_separadas.size(); numero_carpeta++)
    {
        if(resultado_busqueda==true)
        {
            inicio_inodos = inicio_inodos + (numero_inodo *sizeof(inodo));
            // cout<<"buqueda en "<<inicio_inodos<<endl;
            fseek(particion,inicio_inodos,SEEK_SET);
            inodo inodo_lectura;
            fread(&inodo_lectura,sizeof(inodo),1,particion);

            for(int apuntadorD = 0; apuntadorD<12; apuntadorD++)
            {
                if(inodo_lectura.i_block[apuntadorD] != -1)
                {
                    int numero_bloque = inodo_lectura.i_block[apuntadorD];
                    inicio_bloques = inicio_bloques + (numero_bloque*sizeof(bloque_carpetas));
                    fseek(particion,inicio_bloques,SEEK_SET);
                    bloque_carpetas carpetas_lectura;
                    fread(&carpetas_lectura,sizeof(bloque_carpetas),1,particion);

                    int x=0;
                    for(x;x<4; x++)
                    {
                        if(strcmp(carpetas_lectura.b_content[x].b_name,carpetas_separadas[numero_carpeta].c_str())==0)
                        {
                            numero_inodo = carpetas_lectura.b_content[x].b_inodo;
                            // cout<<"numero inodo "<<numero_inodo<<endl;                            
                            inicio_inodos = SP.s_inode_start;
                            inicio_bloques = SP.s_block_start;
                            resultado_busqueda == true;
                            break;
                        }                        
                    }

                    if(x>3)
                    {
                        resultado_busqueda = false;
                    }
                }
            }
        }        
    }


    inicio_inodos = SP.s_inode_start;
    inicio_bloques = SP.s_block_start;
    // ---------------------------------------------------------------
    if(resultado_busqueda==false)
    {
        cout<<"---> La ruta de archivo no exite"<<endl;
    }
    else
    {
        string contenido;
        cout<<"----> Ruta Del archivo Econtrada"<<endl;
        inicio_inodos = inicio_inodos + (numero_inodo * sizeof(inodo));
        fseek(particion,inicio_inodos,SEEK_SET);
        inodo inodo_archivo_econtrado;
        fread(&inodo_archivo_econtrado,sizeof(inodo),1,particion);
        char tipo_inodo = inodo_archivo_econtrado.i_type;
        if(tipo_inodo == '1')
        {
            // recorro todos sus apuntadores directos
            for(int apuntador_directo=0; apuntador_directo<12; apuntador_directo++)
            {
                if(inodo_archivo_econtrado.i_block[apuntador_directo] != -1)
                {
                    int numero_bloque_contenido = inodo_archivo_econtrado.i_block[apuntador_directo];
                    inicio_bloques = inicio_bloques + (numero_bloque_contenido * sizeof(bloque_archivos));
                    fseek(particion,inicio_bloques,SEEK_SET);
                    bloque_archivos contenido_texto;
                    fread(&contenido_texto,sizeof(bloque_archivos),1,particion);
                    contenido += contenido_texto.b_content;
                    inicio_bloques = SP.s_block_start;
                }
            }
        }
        cout<<"--->Contendio del archivo "<<endl;
        cout<<"*********************************"<<endl;
        cout<<endl;
        cout<<contenido<<endl;
        cout<<endl;
        cout<<"**********************************"<<endl;
        cout<<endl;
    }

    // cierre de la particion
    fclose(particion);

}
