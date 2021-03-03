#include "rmdisk.h"

rmdisk::rmdisk()
{

}

/*metodo para la eliminacion de un disco - archivo binario*/
void rmdisk::eliminarDisco(string parametros)
{
    if(parametros.empty() != true)
    {
        this->rutaArchivo = parametros;        
        if(rutaArchivo[0] == '\"')
        {
            this->rutaArchivo = FUN.eliminacionComillas(rutaArchivo);
            confirmacionEliminacion(rutaArchivo);
        }
        else
        {
            confirmacionEliminacion(rutaArchivo);
        }
    }
    else
    { cout<<"-->parametros obligatorios faltantes: path"<<endl; }
}



/*metodo para eleminar un disco - remove()*/
void rmdisk::removerDisco(string rutaArchivo)
{
    if(remove(rutaArchivo.c_str())!=0)
    {cout<<"-->El Disco no es posible eliminar"<<endl;}
    else
    {cout<<"-->Disco Eliminado"<<endl;}
}


/*mensaje para confirmacion de la eliminacion*/
void rmdisk::confirmacionEliminacion(string rutaArchivo)
{
    char confirmacion;
    cout<<"-->Seguro que desea eliminar este disco (Y/N): ";
    cin>>confirmacion;
    if(confirmacion == 'Y' || confirmacion == 'y')
    {
        removerDisco(rutaArchivo);
    }
    else if(confirmacion == 'N' || confirmacion == 'n')
    {}
    else
    {cout<<"-->Comando Invalido"<<endl;}
}
