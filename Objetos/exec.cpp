#include "exec.h"

/*metodo contructor de la clase*/
exec::exec()
{}

/*metodo para lectura del archivo de carga*/
void exec::leerArchivo(string rutaArchivo)
{
    this->rutaArchivo=rutaArchivo;
    if(rutaArchivo.empty() != true)
    {
        if(rutaArchivo[0] == '\"')
        {
            this->rutaArchivo = FUN.eliminacionComillas(rutaArchivo);
            cargarArchivo(rutaArchivo);
        }
        else
        {
            cargarArchivo(rutaArchivo);
        }
    }
    else
    { cout<<"-->Falta parametro obligatorio: path"<<endl;}
}


/*funcion para contenido del archivo*/
void exec::cargarArchivo(string rutaArchivo)
{
    // verificacion de la existencia del archivo
    string lineaComando;
    extern int yyparse(); //
    //static QList<discosmontados> discosenmemoria;
    int p;
    extern int linea; // Linea del token
    extern int columna; // Columna de los tokens
    extern int yylineno;
    bool respuesta = FUN.is_file(rutaArchivo);
    if(respuesta)
    {
        ifstream fichero;
        fichero.open(rutaArchivo.c_str());
        if(!fichero.fail())
        {
            while(!fichero.eof())
            {
                getline(fichero,lineaComando);
                if(lineaComando.empty()==false){
                    cout<<lineaComando<<endl;
                    YY_BUFFER_STATE buffer = yy_scan_string(lineaComando.c_str());
                    linea = 0;
                    columna = 0;
                    yylineno = 0;
                    if(yyparse()!=0) // Si nos da un número negativo, signifca error.
                    {
                        printf("\n\nhay errores\n\n");
                    } 
                }              
            }
            fichero.close();
        }
        cout<<"-->Archivo cargado exitosamente"<<endl;
    }
    else
    {
        cout<<"-->Problema en la apertura del archivo"<<endl;
    }
}
