#include "reportes.h"

reportes::reportes()
{}

// ---- comando principal
void reportes::generarReporte(string parametros[])
{
    if(parametrosObligatorios(parametros))
    {
        // --- revision del tipo reporte sea permitido
        if(verificacionReporte())
        {
            
        }
        else
        {
            cout<<"--->Tipo name (reporte) no reconocido"<<endl;
        }
    }
    else
    {
        cout<<"--->parametros obligatorios faltantes: name, path, id"<<endl;
    }
}

// --- parametros obligatorios para el comando rep
bool reportes::parametrosObligatorios(string parametros[])
{
    // --- name posicion 0, path posicion 1, id posicion 2
    if(parametros[0].empty()!=true)
    {
        this->name = FUN.aMinuscula(parametros[0]);
        return true;
    }
    else
    {
        return false;
    }
}



// ---- verificacion tipo de reporte es aceptado
bool reportes::verificacionReporte()
{
    // --- recorrido
    for(int reporte = 0; reporte<reportesPermitidos.size(); reporte++)
    {
        if(this->name == reportesPermitidos[reporte])
        {
            cout<<reportesPermitidos[reporte]<<endl;
            return true;
        }        
    }
    return false;
}