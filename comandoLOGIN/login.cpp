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
    superBloque SPlectura;
    FILE *archivo;
    // apertura del disco para lectura y actualizacion rb+
    archivo = fopen(rutaArchivo.c_str(),"rb+");    
    if(archivo==NULL)
        exit(1);
    // lectura del SUPERBLOQUE y su informacion
    fseek(archivo,part_star,SEEK_SET);
    fread(&SPlectura,sizeof(SPlectura),1,archivo);
    // incio de los bloques escritos en la particion
    int inicioBloques = SPlectura.s_block_start;
    cout<<"Inicio de los Bloque en la particon es: "<<inicioBloques<<endl;

    //---posicion en el inicio de los bloque para buscar el bloque con la informaciond e los usuarios
    fseek(archivo,inicioBloques,SEEK_SET);
    inicioBloques = inicioBloques + sizeof(bloque_carpetas);
    fseek(archivo,inicioBloques,SEEK_SET);//incio del bloque user
    // bloque user
    bloque_archivos archivoUser;
    fread(&archivoUser,sizeof(archivoUser),1,archivo);
    // ------Busqueda del para inicio de sescion de un usuario
    string contenidoBloque = FUN.convertirArreglochar(archivoUser.b_content);
    if(FUN.buscarDentroVector(contenidoBloque,usr) && FUN.buscarDentroVector(contenidoBloque,pass))
    {
        cout<<endl;
        cout<<"--->Session Activa<---"<<endl;
        // agregacon del usuario a la lista
        addUsuario(listaUsuarios,usr,pass,identificador);
    }
    else
    {
        cout<<endl;
        cout<<"--->El usuario no existe<---"<<endl;
    }
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
