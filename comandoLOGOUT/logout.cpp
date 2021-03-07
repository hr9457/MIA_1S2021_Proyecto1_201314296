#include "logout.h"

logout::logout()
{}


// funcion para una sesion activa
void logout::cerrarSession(vector<usuarioConectado>&listado)
{
    // verificacion de un usuario activo
    if(listado.size() > 0)
    {
        // elimno todos los elementos en la session
        listado.clear();
        cout<<"--->Session Cerrada<---"<<endl;
    }
    else
    {
        cout<<endl;
        cout<<"--->No hay ninguna session Activa<---"<<endl;
    }
}
