#include "login.h"

login::login()
{}


// funcion para el recibimiento de los parametros para el comando login
void login::iniciarSession(vector<usuarioConectado>&listado,vector<montajeDisco>&listadoDiscos,string parametros[])
{   
    if(listado.size() < 1)
    {
        // verificacion y actualizacion
        if(verificacionParametros(parametros))
        {   
            // existencia del identificador
            if(FUN.busquedaParticion(listadoDiscos,this->identificador))
            {
                //addUsuario(listado,this->usuario,this->password,this->identificador);
                verificacionUsuario(listadoDiscos,this->identificador,this->usuario,this->password,listado);
            }
            else
            {
                cout<<"--->El id del Disco no se encuntra montado<---"<<endl;
            }
        }
    }
    else
    {
        cout<<endl;
        cout<<"---->Ya hay un usuario con Session activa<----"<<endl;
    }
}


// verificion de parametros para el comando login todos son obligatorios
bool login::verificacionParametros(string parametros[])
{
    // usuario posicion 0, password posicion 1, identificador posicion 2
    if(parametros[0].empty()!= true && parametros[1].empty()!=true && parametros[2].empty()!= true)
    {
        // usuario
        this->usuario = eliminacionComillas(parametros[0]);
        // password
        this->password = eliminacionComillas(parametros[1]);
        // identificador
        this->identificador = parametros[2];
        return true;
    }
    else
    {
        cout<<"--->Parametros Obligatorios faltantes: usr, pwd, id<---"<<endl;
        return false;
    }
}



// eliminacion de comillas en los parametros
string login::eliminacionComillas(string palabra)
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




// metodo para la verficacon de la existencia del usuario en la particion
void login::verificacionUsuario(vector<montajeDisco>&listado,string identificador,string usr,string pass,vector<usuarioConectado>&listaUsuarios)
{
    string rutaArchivo = FUN.busquedaPathParticion(listado,identificador);
    int part_star = FUN.busquedaStarParticion(listado,identificador);
    // cout<<"Ruta del archivo de lectura "<<rutaArchivo<<endl;
    // cout<<"Inicio de la particion donde hacer el login "<<part_star<<endl;
    
    FILE *archivo;
    archivo = fopen(rutaArchivo.c_str(),"rb+");
    if(archivo==NULL)
        exit(1);

    superBloque SP;
    fseek(archivo,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,archivo);

    inodo raiz;
    fseek(archivo,SP.s_inode_start,SEEK_SET);
    fread(&raiz,sizeof(raiz),1,archivo);

    // ----- algoritmo de busueda para el inodo
    //--------lo que voy a buscar
    int inicio_Inodos = SP.s_inode_start;
    int inicio_Bloques = SP.s_block_start;
    bloque_carpetas bloque_busqueda;
    char archivoUser[] = "user.txt"; 
    int inodoDelArchivo=-1;  

    // ------------ Algoritmo de busqueda del inodo de lo que se quiere buscar
    // ------ busqueda en cada apuntador directo
    for(int apuntador=0; apuntador<12; apuntador++)
    {
        if(raiz.i_block[apuntador] != -1)
        {
            // ---- numero de bloque localizado
            int numero_bloque = raiz.i_block[apuntador];
            // cout<<"***Numero bloque a revisar"<<numero_bloque<<endl;

            // ------ me ubico en ese bloque
            if(numero_bloque == 0)
            {
                // cout<<"***Boque cero"<<endl;
                //----- bloque cero
                fseek(archivo,inicio_Bloques,SEEK_SET);
                fread(&bloque_busqueda,sizeof(bloque_busqueda),1,archivo);
                //------ busqueda en cada apuntador del bloque
                for(int i=0;i<4;i++)
                {
                    //cout<<"***Bloque apuntador "<<i<<endl;
                    if(strcmp(bloque_busqueda.b_content[i].b_name,archivoUser)==0)
                    {
                        // //---- inodo donde esta el archivo user.txt
                        // cout<<bloque_busqueda.b_content[i].b_name<<endl;
                        // cout<<bloque_busqueda.b_content[i].b_inodo<<endl;
                        inodoDelArchivo = bloque_busqueda.b_content[i].b_inodo;
                        break;
                    }//fin if

                }//fin for
                //---- reinicio 
                inicio_Bloques = SP.s_block_start;

            }// fin del if ==0
            else
            {
                //---- ubicacion en el bloque a revisar
                inicio_Bloques = numero_bloque * sizeof(bloque_carpetas);
                fseek(archivo,inicio_Bloques,SEEK_SET);
                fread(&bloque_busqueda,sizeof(bloque_busqueda),1,archivo);
                // ----- busqueda en cada apuntador de ese bloque
                for(int i=0;i<4;i++)
                {
                    if(strcmp(bloque_busqueda.b_content[i].b_name,archivoUser)==0)
                    {
                        inodoDelArchivo = bloque_busqueda.b_content[i].b_inodo;
                        break;
                    }
                }
                //---- reinicio 
                inicio_Bloques = SP.s_block_start;

            }

        }// fin del if verifica que nosea -1
    }// fin del for principal


    // cout<<"Inodo del archivo: "<<inodoDelArchivo<<endl;
    //------busqueda en los apuntadores de ese inodo
    int inodoEnontrado = SP.s_inode_start + (inodoDelArchivo * sizeof(inodo));
    bool usuarioEncontrado = false;

    // ----- busco en en cada apuntador del nodo
    inodo inodoArchivoUsr;
    fseek(archivo,inodoEnontrado,SEEK_SET);
    fread(&inodoArchivoUsr,sizeof(inodoArchivoUsr),1,archivo);
    for(int apU=0;apU<12;apU++)
    {
        if(inodoArchivoUsr.i_block[apU] != -1)
        {
            bloque_archivos archivoDeUsuarios;
            int inicioBusquedaArchivo = SP.s_block_start + (inodoArchivoUsr.i_block[apU]*sizeof(bloque_archivos));
            fseek(archivo,inicioBusquedaArchivo,SEEK_SET);
            fread(&archivoDeUsuarios,sizeof(archivoDeUsuarios),1,archivo);
            // cout<<archivoDeUsuarios.b_content<<endl;
            // ---- saco el contenido del bloque para buscar dentro de el
            string contenidoBloque = FUN.convertirArreglochar(archivoDeUsuarios.b_content);
            if(FUN.buscarDentroVector(contenidoBloque,usr) && FUN.buscarDentroVector(contenidoBloque,pass))
            {
                cout<<endl;
                cout<<"--->Session Activa<---"<<endl;
                cout<<"--->Usuario y password coinciden<---"<<endl;
                // agregacon del usuario a la lista
                usuarioEncontrado = true;
                addUsuario(listaUsuarios,usr,pass,identificador);
                break;                
            }
        }
    }

    if(usuarioEncontrado==false)
    {
        cout<<endl;
        cout<<"--->El usuario no existe<---"<<endl;
    }

    fclose(archivo); 
}


// agregar el usuario a la lista de usuario conectados
void login::addUsuario(vector<usuarioConectado>&listado,string usr,string pwd,string identificador)
{
    usuarioConectado nuevoUsuario;
    strcpy(nuevoUsuario.usuario,usr.c_str());
    strcpy(nuevoUsuario.password,pwd.c_str());
    strcpy(nuevoUsuario.identificador,identificador.c_str());
    listado.push_back(nuevoUsuario);
    // ----Impresion de los usuarios montados
    cout<<"--->Listado de los usuarios Activos<---"<<endl;
    for(int usuario=0; usuario<listado.size(); usuario++)
    {
        cout<<"--->Usuario: "<<listado[usuario].usuario<<endl;
        cout<<"--->Id de Disco: "<<listado[usuario].identificador<<endl<<endl;
    }
}
