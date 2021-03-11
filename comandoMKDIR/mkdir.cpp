#include "mkdir.h"

mkdir::mkdir()
{
}


// ---- comando principal
void mkdir::crearCarpeta(vector<usuarioConectado>&usrConectados,string parametros[])
{

    // ---- verificacion de algun inicio de session de algun usuario
    if(verificacionSession(usrConectados))
    {
        // ---- verificacion del parametro obligatorio path
        if(parametroObligatorio(parametros))
        {
            // ---- verificacion del parametro opcional
            // if(parametrosOpcional(parametros))
            // {    
            // }
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
bool mkdir::parametroObligatorio(string parametros[])
{
    if(parametros[0].empty()!=true)
    {
        this->rutaArchivo = eliminacionComillas(parametros[0]);
        return true;
    }
    return false;
}


// ---- eliminacion de comillas de una ruta
string mkdir::eliminacionComillas(string palabra)
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
bool mkdir::verificacionSession(vector<usuarioConectado>&usuarioConectados)
{
    if(usuarioConectados.size()>0)
    {
        return true;
    }
    return false;
}


// ----- verificacion del parametro opcional
bool mkdir::parametrosOpcional(string parametos[])
{
    if(parametos[1].empty()!=true)
    {
        if(parametos[1] == "true")
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
