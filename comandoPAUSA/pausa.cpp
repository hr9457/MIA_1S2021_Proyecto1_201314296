#include "pausa.h"

pausa::pausa()
{}

// ---- comando para pusar el sistema
void pausa::pausarSistema()
{
    int c;
    cout<<"--->Presione cualquier tecla pra continuar: ";
    c = getchar();
}
