#include "mkfile.h"

mkfile::mkfile()
{
}
// ---- comando principal
void mkfile::crear(string parametros[])
{
    // --- revision del parametro path obligatorios
    if(parametros[0].empty() != true)
    {
        // ---- verificacion de comillas en la ruta de entrada
        this->rutaArchivo = parametros[0];
        this->rutaArchivo = verificacionComillas(this->rutaArchivo);
        
    }
    else
    {
        cout<<"--->Parametro obligatorio faltante: path"<<endl;
    }
}


// --- comando de verificacion de comilas en el archivo
string mkfile::verificacionComillas(string rutaArchivo)
{
    if(rutaArchivo[0] == '\"')
    {
        return FUN.eliminacionComillas(rutaArchivo);
    }
    else
    {
        return rutaArchivo;
    }
}