#include "comamkdir.h"

comamkdir::comamkdir()
{
}


// ---- comando principal
void comamkdir::crearCarpeta(vector<usuarioConectado>&usrConectados,vector<montajeDisco>&discos,string parametros[])
{

    // ---- verificacion de algun inicio de session de algun usuario
    if(verificacionSession(usrConectados))
    {
        // ---- verificacion del parametro obligatorio path
        if(parametroObligatorio(parametros))
        {
            // ------------ parametro opcional
            parametrosOpcional(parametros);
            // -------------- buscqueda de carpetas
            buscarCrearCarpeta(usrConectados,discos);
        }
        else
        {
            cout<<"--->Parametro Obligatorio faltante: path"<<endl;
        }
    }
    else
    {
        cout<<"--->NO HAY NINGUNA SESSION INICIADA"<<endl;
    }    
}


// ----- verificacion de parametros
bool comamkdir::parametroObligatorio(string parametros[])
{
    if(parametros[0].empty()!=true)
    {
        this->rutaArchivo = eliminacionComillas(parametros[0]);
        return true;
    }
    return false;
}


// ---- eliminacion de comillas de una ruta
string comamkdir::eliminacionComillas(string palabra)
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


// --- verificacion de inicio de session
bool comamkdir::verificacionSession(vector<usuarioConectado>&usuarioConectados)
{
    if(usuarioConectados.size()>0)
    {
        return true;
    }
    return false;
}


void comamkdir::parametrosOpcional(string paarmetros[])
{
    if(paarmetros[1].empty()!=true)
    {
        this->valorP = "true";
    }
}



void comamkdir::buscarCrearCarpeta(vector<usuarioConectado>&usuarioConectado,vector<montajeDisco>&discos)
{
    string identificador = usuarioConectado[0].identificador;
    string rutaArchivo = FUN.busquedaPathParticion(discos,identificador);
    int part_star = FUN.busquedaStarParticion(discos,identificador);
    string carpeta_a_buscar = "";
    string carpetas_multiple = this->rutaArchivo;
    vector<string> carpetas_separadas = FUN.split(carpetas_multiple,'/');

    // ------- Apertura
    FILE *discoLectura;
    discoLectura = fopen(rutaArchivo.c_str(),"rb+");
    // ----------------------------
    superBloque SP;
    fseek(discoLectura,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,discoLectura);
    // -------------------------- datos ---------------------------------
    int inicio_bm_inodos = SP.s_bm_inode_start;
    int inicio_bm_bloques = SP.s_bm_block_start;
    int inicio_inodos = SP.s_inode_start;
    int inicio_bloques = SP.s_block_start;
    int inodos_usados = SP.s_firts_ino;
    int inodo_permitido_libre = SP.s_firts_ino;
    int inodo_libre_para_usar = SP.s_firts_ino;
    int inodo_libre_para_usar2 = inodo_libre_para_usar;
    int bloque_libre_para_usar = SP.s_first_blo;
    int bloque_libre_para_usar2 = bloque_libre_para_usar;


    // -----------------------------
    int espacio_bloque_carpetas = -1; // -1 no hay espacio la carpeta de bloque
    int numero_bloque_espacio_bloque_carpetas = -1;
    int numero_inodo_busqueda = 0; // inodo raiz
    int apuntador_disponible_inodo = -1;
    int inodo_reslutado_busqueda;
    int inodo_padre;
    int numero_bloque_actualizar;

    //  ------ revision de la ruta para la busqueda o creacion de la carpeta
    int numero_carpeta=1;
    // -1 no se econtro la carpeta en el inodo que se esta buscando
    // ------------------------
    while(numero_inodo_busqueda!= -1)
    {
        // recorrer carpetas por carpetas
        inodo_padre = numero_inodo_busqueda;
        carpeta_a_buscar = carpetas_separadas[numero_carpeta];
        int copia_numero_inodo_buqueda = numero_inodo_busqueda;
        numero_inodo_busqueda = FUN.buscarInodoArchivoCarpeta2(discoLectura,SP,numero_inodo_busqueda,carpeta_a_buscar,espacio_bloque_carpetas);
        numero_bloque_espacio_bloque_carpetas = FUN.espacioLibreBloqueInodo(discoLectura,SP,copia_numero_inodo_buqueda);
        numero_bloque_actualizar = FUN.espacioLibreBloqueInodo2(discoLectura,SP,copia_numero_inodo_buqueda);
        // FUN.espacioLibreInodo(discoLectura,SP,numero_inodo_busqueda,apuntador_disponible_inodo);

        cout<<"inodo padre: "<<inodo_padre<<endl;
        cout<<"carpeta donde buscar: "<<carpeta_a_buscar<<endl;
        cout<<"numero inodo busqueda: "<<numero_inodo_busqueda<<endl;
        cout<<"espacio bloque del inodo: "<<numero_bloque_espacio_bloque_carpetas<<endl;
        // cout<<"apuntador disponible en el inodo: "<<apuntador_disponible_inodo<<endl;
        
        // ---- decicion para crear o no
        if(this->valorP == "true" && numero_inodo_busqueda == -1)
        {
            cout<<endl;
            cout<<"---> SE HA CREADO LA CARPETA **> "<<carpeta_a_buscar<<" <** EN EL SISTEMA "<<carpeta_a_buscar<<endl;
            // ----------- crear carpeta
            // -- movimiento al primer inodo libre en la particion
            
            // creo el inodo carpeta
            inodo nuevo_inodo_carpeta;
            nuevo_inodo_carpeta.i_uid = 1;
            nuevo_inodo_carpeta.i_gid = 1;
            nuevo_inodo_carpeta.i_size = 0;
            strcpy(nuevo_inodo_carpeta.i_atime,FUN.obtenerFechaHora().c_str());
            strcpy(nuevo_inodo_carpeta.i_ctime,FUN.obtenerFechaHora().c_str());
            strcpy(nuevo_inodo_carpeta.i_mtime,FUN.obtenerFechaHora().c_str());
            for(int apuntador=0;apuntador<15;apuntador++)
            {
                nuevo_inodo_carpeta.i_block[apuntador] = -1;
            }
            nuevo_inodo_carpeta.i_type = '0';
            nuevo_inodo_carpeta.i_perm = 664;
            // --------------------------------------------------------------------------------
            // ------ le creo su primer bloque de apuntadores
            bloque_carpetas carpetas_inodo_nuevo;
            content contenido_nuevo_inodo;
            // ---- 0
            strcpy(contenido_nuevo_inodo.b_name,".");
            contenido_nuevo_inodo.b_inodo = inodo_libre_para_usar;
            carpetas_inodo_nuevo.b_content[0] = contenido_nuevo_inodo;
            // ---- 1
            strcpy(contenido_nuevo_inodo.b_name,"..");
            contenido_nuevo_inodo.b_inodo = inodo_padre;
            carpetas_inodo_nuevo.b_content[1] = contenido_nuevo_inodo;
            // ---- 2
            strcpy(contenido_nuevo_inodo.b_name,"");
            contenido_nuevo_inodo.b_inodo = -1;
            carpetas_inodo_nuevo.b_content[2] = contenido_nuevo_inodo;
            // ---- 3
            strcpy(contenido_nuevo_inodo.b_name,"");
            contenido_nuevo_inodo.b_inodo = -1;
            carpetas_inodo_nuevo.b_content[3] = contenido_nuevo_inodo;
            // ---- lo ligo a inodo nuevo creado
            nuevo_inodo_carpeta.i_block[0] = bloque_libre_para_usar;

            
            //-----------------------------------------
            // ligar el nuevo inodo a un bloque
            if(numero_bloque_espacio_bloque_carpetas!=-1)
            {
                // espacio libre en el bloque
                cout<<numero_bloque_espacio_bloque_carpetas<<endl;
                cout<<"bloque: "<<numero_bloque_actualizar<<endl;
                // tengo que mover a ese bloque
                inicio_bloques = inicio_bloques + (numero_bloque_actualizar * sizeof(bloque_carpetas));
                fseek(discoLectura,inicio_bloques,SEEK_SET);

                bloque_carpetas espcio_bloque_carpetas_inodoPadre;
                fread(&espcio_bloque_carpetas_inodoPadre,sizeof(espcio_bloque_carpetas_inodoPadre),1,discoLectura);

                // ----------------------------------------------
                content nuevo_apuntador_inodoPadre;
                strcpy(nuevo_apuntador_inodoPadre.b_name,carpeta_a_buscar.c_str());
                nuevo_apuntador_inodoPadre.b_inodo = inodo_libre_para_usar;

                // ---------------3
                espcio_bloque_carpetas_inodoPadre.b_content[numero_bloque_espacio_bloque_carpetas] = nuevo_apuntador_inodoPadre;

                for(int i=0;i<4;i++)
                {
                    cout<<espcio_bloque_carpetas_inodoPadre.b_content[i].b_name<<"<-->";
                    cout<<espcio_bloque_carpetas_inodoPadre.b_content[i].b_inodo<<endl;
                }

                // ------- los escirbo y actualizo el bloque
                fseek(discoLectura,inicio_bloques,SEEK_SET);
                fwrite(&espcio_bloque_carpetas_inodoPadre,sizeof(espcio_bloque_carpetas_inodoPadre),1,discoLectura);   
                // ------------------------
                // reinicio
                // ------------------------------------------------
                // reinicio posicion inciales de star
                inicio_inodos = SP.s_inode_start;
                inicio_bloques = SP.s_block_start;


                // -------------------------------------------------------------------------------------------------------
                // ESCRITURA EN EL DISCO DURO
                //  ------- escirbo el nuevo inodo
                inicio_inodos = inicio_inodos + ( inodo_libre_para_usar * sizeof(inodo) );
                fseek(discoLectura,inicio_inodos,SEEK_SET);
                fwrite(&nuevo_inodo_carpeta,sizeof(nuevo_inodo_carpeta),1,discoLectura);
                // -------- escirbo su bloque
                inicio_bloques = inicio_bloques + ( bloque_libre_para_usar * sizeof(bloque_carpetas) );
                fseek(discoLectura,inicio_bloques,SEEK_SET);
                fwrite(&carpetas_inodo_nuevo,sizeof(carpetas_inodo_nuevo),1,discoLectura);
                // ------------------------------------------------
                // reinicio posicion inciales de star
                inicio_inodos = SP.s_inode_start;
                inicio_bloques = SP.s_block_start;

                // -------------------------
                // ----- Escribo en los bitmap
                char bm_ocupado = '1';
                // bitmap inodos
                inicio_bm_inodos = inicio_bm_inodos + inodo_libre_para_usar;
                fseek(discoLectura,inicio_bm_inodos,SEEK_SET);
                fwrite(&bm_ocupado,sizeof(bm_ocupado),1,discoLectura);
                // bitmap bloques
                inicio_bm_bloques = inicio_bm_bloques + bloque_libre_para_usar;
                fseek(discoLectura,inicio_bm_bloques,SEEK_SET);
                fwrite(&bm_ocupado,sizeof(bm_ocupado),1,discoLectura);
                // -----------------------------------
                //  reinicio posiciones inciales bm
                inicio_bm_inodos = SP.s_bm_inode_start;
                inicio_bm_bloques = SP.s_bm_block_start;


            }
            // else
            // {

            //     if(apuntador_disponible_inodo!=-1 && apuntador_disponible_inodo<12)
            //     {
            //         inicio_inodos = inicio_inodos + ( inodo_padre * sizeof(inodo) );
            //         fseek(discoLectura,inicio_inodos,SEEK_SET);
            //         inodo inodo_padre_lectura;
            //         fread(&inodo_padre_lectura,sizeof(inodo_padre_lectura),1,discoLectura);
            //         inodo_padre_lectura.i_block[apuntador_disponible_inodo] = inodo_libre_para_usar;
            //         // ----------------------------------------------------------------------------------
            //         // actualizo
            //         fseek(discoLectura,inicio_inodos,SEEK_SET);
            //         fwrite(&inodo_padre_lectura,sizeof(inodo_padre_lectura),1,discoLectura);
            //         // ------------------------
            //         // reinicio
            //         // ------------------------------------------------
            //         // reinicio posicion inciales de star
            //         inicio_inodos = SP.s_inode_start;
            //         inicio_bloques = SP.s_block_start;
            //         // -------------------------------------------------------------------------------------------------------
            //         // ESCRITURA EN EL DISCO DURO
            //         //  ------- escirbo el nuevo inodo
            //         inicio_inodos = inicio_inodos + ( inodo_libre_para_usar * sizeof(inodo) );
            //         fseek(discoLectura,inicio_inodos,SEEK_SET);
            //         fwrite(&nuevo_inodo_carpeta,sizeof(nuevo_inodo_carpeta),1,discoLectura);
            //         // -------- escirbo su bloque
            //         inicio_bloques = inicio_bloques + ( bloque_libre_para_usar * sizeof(bloque_carpetas) );
            //         fseek(discoLectura,inicio_bloques,SEEK_SET);
            //         fwrite(&carpetas_inodo_nuevo,sizeof(carpetas_inodo_nuevo),1,discoLectura);
            //         // ------------------------------------------------
            //         // reinicio posicion inciales de star
            //         inicio_inodos = SP.s_inode_start;
            //         inicio_bloques = SP.s_block_start;

            //         // -------------------------
            //         // ----- Escribo en los bitmap
            //         char bm_ocupado = '1';
            //         // bitmap inodos
            //         inicio_bm_inodos = inicio_bm_inodos + inodo_libre_para_usar;
            //         fseek(discoLectura,inicio_bm_inodos,SEEK_SET);
            //         fwrite(&bm_ocupado,sizeof(bm_ocupado),1,discoLectura);
            //         // bitmap bloques
            //         inicio_bm_bloques = inicio_bm_bloques + bloque_libre_para_usar;
            //         fseek(discoLectura,inicio_bm_bloques,SEEK_SET);
            //         fwrite(&bm_ocupado,sizeof(bm_ocupado),1,discoLectura);
            //         // -----------------------------------
            //         //  reinicio posiciones inciales bm
            //         inicio_bm_inodos = SP.s_bm_inode_start;
            //         inicio_bm_bloques = SP.s_bm_block_start;

            //     }
            //     else
            //     {
            //         cout<<"----->No se ha creado la carpeta ya no hay espacio"<<endl;
            //     }

            // }


            // actualizar inodo libre y bloque libre
            bloque_libre_para_usar2 = inodo_libre_para_usar;
            inodo_libre_para_usar++;
            bloque_libre_para_usar++;


        }
        else
        {
            cout<<endl;
            cout<<"---> ERROR LAS CARPETA NO EXISTE EN EL SISTEMA"<<endl;
        }

        cout<<"---------->   "<<numero_inodo_busqueda<<endl;
        numero_carpeta++;
        // -------- reviso si ya llegue al final de todas las carpetas
        if(numero_inodo_busqueda==-1 && numero_carpeta<carpetas_separadas.size())
        {
            numero_inodo_busqueda = bloque_libre_para_usar2;
        }
    }

    // for(int i=0;i<carpetas_separadas.size();i++)
    // {
    //     cout<<i<<"<--->"<<carpetas_separadas[i]<<endl;
    // }
    // -------------------------------------------------------
    // hay que actualizar el super bloque
    // ----------------------------
    superBloque SP_actualizado;
    fseek(discoLectura,part_star,SEEK_SET);
    fread(&SP_actualizado,sizeof(SP_actualizado),1,discoLectura);
    SP_actualizado.s_firts_ino = inodo_libre_para_usar;
    SP_actualizado.s_first_blo = bloque_libre_para_usar;
    fseek(discoLectura,part_star,SEEK_SET);
    fwrite(&SP_actualizado,sizeof(SP_actualizado),1,discoLectura);

    // ---------------------------------------------------------------------------------------------------------
    cout<<"---------> Se termino el ciclo"<<endl;    
    // --- cierre
    fclose(discoLectura);
    cout<<endl;
}