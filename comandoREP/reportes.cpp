#include "reportes.h"

reportes::reportes()
{}

// ---- comando principal
void reportes::generarReporte(string parametros[],vector<usuarioConectado>&usuarios,vector<montajeDisco>&discos)
{
    if(usuarioLogeado(usuarios))
    {
        if(parametrosObligatorios(parametros))
        {
            // cout<<"parametros obligatorios"<<endl;
            // --- revision del tipo reporte sea permitido
            if(verificacionReporte())
            {
                generarTipoReporte(this->name,usuarios,discos);
                cout<<endl;
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
    else
    {
        cout<<"--->NO EXISTE NINGUN USUARIO LOGEADO"<<endl;
    }
}



// --- parametros obligatorios para el comando rep
bool reportes::parametrosObligatorios(string parametros[])
{
    // --- name posicion 0, path posicion 1, id posicion 2
    if(parametros[0].empty()!=true && parametros[1].empty()!=true)
    {
        this->name = FUN.aMinuscula(parametros[0]);
        this->path = eliminacionComillas(parametros[1]);
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
            // cout<<reportesPermitidos[reporte]<<endl;
            return true;
        }        
    }
    return false;
}



// ---- eliminacion de comillas de una ruta
string reportes::eliminacionComillas(string palabra)
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



// --- verificacion y creacion de carpetas
void reportes::crearCarpetas(string nombreCarpeta)
{
    if(mkdir(nombreCarpeta.c_str(),07777)==-1)
    {
        cout<<"-->Carpeta Existente\n";
    }
}



// ---- verificacion de algun usuario logeado para la generacion de reportes
bool reportes::usuarioLogeado(vector<usuarioConectado>&usuarios)
{
    if(usuarios.size()>0)
    {
        return true;
    }
    else
    {
        return false;
    }
}



// ----- funcion para descomponer la ruta por /
string reportes::descomponerRuta(string rutaArchivo)
{
    vector<string>rutas = FUN.split(rutaArchivo,'/');
    string rutaCarpeta;
    string nombreArchivo;
    int i=0;
    for(i;i<rutas.size()-1;i++)
    {
        rutaCarpeta =  rutaCarpeta + rutas[i] + "/";
        // cout<<rutas[i]<<endl;
    }
    this->nombreArchivoDot = rutas.at(i);
    return rutaCarpeta;
}



void reportes::generarTipoReporte(string reporte,vector<usuarioConectado>&usuarios,vector<montajeDisco>&discos)
{
    string identificador = usuarios[0].identificador;
    string rutaArchivo = FUN.busquedaPathParticion(discos,identificador);
    int part_star = FUN.busquedaStarParticion(discos,identificador);
    if(reporte == "mbr")
    {
        reporteMBR(rutaArchivo);
    }
    else if(reporte == "disk")
    {
    }
    else if(reporte == "bm_inode")
    {
        reportBMindode(rutaArchivo,part_star);
    }
    else if(reporte == "bm_block")
    {
        reportBMbloque(rutaArchivo,part_star);
    }
    else if(reporte == "sb")
    {
        reportSP(rutaArchivo,part_star);
    }
}



// --- para generara reporte del mbr
void reportes::reporteMBR(string rutaArchivo)
{
    // -----------
    FILE *discoLectura;
    discoLectura = fopen(rutaArchivo.c_str(),"rb+");
    if(discoLectura==NULL)
    {
        fclose(discoLectura);
        exit(1);        
    }

    // --- lectura del mbr del disco
    mbr MBR;
    fseek(discoLectura,0,SEEK_SET);
    fread(&MBR,sizeof(MBR),1,discoLectura);
    int mbr_tamanio = MBR.mbr_tamanio;
    string mbr_fecha = MBR.mbr_fecha_creacion;
    int mbr_disk_signature = MBR.mbr_disk_signature;
    char mbr_fit = MBR.mbr_fit;

    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    // cout<<ruta<<endl;
    // cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    // --- creacion del archivo dot 
    string ruta_creacion_dot = ruta + "mbr.dot";
    ofstream archivo;
    archivo.open(ruta_creacion_dot);
    if(archivo.fail())
    {
        cout<<"--->Error en el archivo reporte MBR"<<endl;
        archivo.close();
        fclose(discoLectura);
    }

    archivo<<"digraph{"<<endl;
    archivo<<" graph [pad=\"0.5\",nodesep=\"0.5\",ranksep=\"2\"]; "<<endl;
    archivo<<" node [shape=plain]"<<endl;
    archivo<<" rankdir=LR;"<<endl;
    // ----- escirtura de la informacion del MBR
    archivo<<"MBR [label=<"<<endl;
    archivo<<"<table borde=\"0\" cellborde=\"1\" cellspacing=\"0\">"<<endl;
    // ---- informacion dle MBR leido
    archivo<<"<tr><td><i>Nombre</i></td><td><i>Valor</i></td></tr>"<<endl;
    archivo<<"<tr><td>mbr_tamano</td><td>"<<mbr_tamanio<<"</td></tr>"<<endl;
    archivo<<"<tr><td>mbr_fecha</td><td>"<<mbr_fecha<<"</td></tr>"<<endl;
    archivo<<"<tr><td>mbr_signature</td><td>"<<mbr_disk_signature<<"</td></tr>"<<endl;
    archivo<<"<tr><td>mbr_fit</td><td>"<<mbr_fit<<"</td></tr>"<<endl;
    for(int i=0;i<4;i++)
    {
        archivo<<"<tr><td>part_status_"<<i+1<<"</td><td>"<<MBR.mbr_partitions[i].part_status<<"</td></tr>"<<endl;
        archivo<<"<tr><td>part_type_"<<i+1<<"</td><td>"<<MBR.mbr_partitions[i].part_type<<"</td></tr>"<<endl;
        archivo<<"<tr><td>part_fit_"<<i+1<<"</td><td>"<<MBR.mbr_partitions[i].part_fit<<"</td></tr>"<<endl;
        archivo<<"<tr><td>part_star_"<<i+1<<"</td><td>"<<MBR.mbr_partitions[i].part_star<<"</td></tr>"<<endl;
        archivo<<"<tr><td>part_size_"<<i+1<<"</td><td>"<<MBR.mbr_partitions[i].part_size<<"</td></tr>"<<endl;
        string nombre_particion = MBR.mbr_partitions[i].part_name;
        archivo<<"<tr><td>part_type_"<<i+1<<"</td><td>"<<nombre_particion<<"</td></tr>"<<endl;
    }    
    // ------------------------------
    archivo<<"</table>>];"<<endl;
    archivo<<"}"<<endl;
    archivo.close();

    // ----- cierre del disco
    fclose(discoLectura);


    // ---- conversion del archivo a tipo 
    if(this->nombreArchivoDot != "")
    {
        vector<string> nombre_extesion = FUN.split(nombreArchivoDot,'.');
        string tipo = tipoConversion(nombre_extesion[1]);
        string comando = "dot " + tipo + " " + ruta_creacion_dot + " -o " + this->path;
        // cout<<comando<<endl;
        system(comando.c_str());
    }
    else
    {
        cout<<"--->No se ha expecifica el tipo de extension"<<endl;
    }    
}


string reportes::tipoConversion(string tipo)
{
    if(tipo == "png")
    {
        return "-Tpng";
    }
    else if(tipo == "jpg")
    {
        return "-jpg";
    }
    else if(tipo == "pdf")
    {
        return "-pdf";
    }
    else
    {
        return "pdf";
    }
}



// ---- reporte del bitmap de inodos
void reportes::reportBMindode(string rutaArchivo,int part_star)
{
    // -----------
    FILE *discoLectura;
    discoLectura = fopen(rutaArchivo.c_str(),"rb+");
    if(discoLectura==NULL)
    {
        fclose(discoLectura);
        exit(1);        
    }

    // ---- incio de la particion 
    superBloque SP;
    cout<<"inicio de la particion es: "<<part_star<<endl;
    fseek(discoLectura,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,discoLectura);

    int inicio_bm_inodos = SP.s_bm_inode_start;
    int inicio_bm_bloques = SP.s_bm_block_start;
    int total_bitmap = inicio_bm_bloques - inicio_bm_inodos;
    // cout<<inicio_bm_inodos<<endl;
    // cout<<inicio_bm_bloques<<endl;
    
    // ---- 20 registros por linea
    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    cout<<ruta<<endl;
    cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    
    string ruta_creacion_txt = this->path;
    ofstream archivo;
    archivo.open(ruta_creacion_txt);
    if(archivo.fail())
    {
        cout<<"--->Error en el archivo reporte MBR"<<endl;
        archivo.close();
        fclose(discoLectura);
    }

    // ---- recorrer todo el bitmap
    fseek(discoLectura,inicio_bm_inodos,SEEK_SET);
    char lectura_bit;
    int contador_de_bitmap = 0;
    for(int bitmap = 0; bitmap<total_bitmap; bitmap++)
    {
        if(contador_de_bitmap<20)
        {
            fread(&lectura_bit,sizeof(lectura_bit),1,discoLectura);
            archivo<<lectura_bit<<"-";
            contador_de_bitmap++;
        }
        else
        {
            archivo<<endl;
            fread(&lectura_bit,sizeof(lectura_bit),1,discoLectura);
            archivo<<lectura_bit<<"-";
            contador_de_bitmap=1;
        }
        inicio_bm_inodos++;
        fseek(discoLectura,inicio_bm_inodos,SEEK_SET);
    }
    archivo.close();

    fclose(discoLectura);    
}




// ----- reporte para el bitmap de bloque de una particion
void reportes::reportBMbloque(string rutaArchivo,int part_star)
{
    // -----------
    FILE *discoLectura;
    discoLectura = fopen(rutaArchivo.c_str(),"rb+");
    if(discoLectura==NULL)
    {
        fclose(discoLectura);
        exit(1);        
    }

    // ---- incio de la particion 
    superBloque SP;
    cout<<"inicio de la particion es: "<<part_star<<endl;
    fseek(discoLectura,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,discoLectura);

    // ----- datos
    int inicio_bm_bloques = SP.s_bm_block_start;
    int tota_bm_bloques = SP.s_inode_start - SP.s_bm_block_start;
    // cout<<inicio_bm_inodos<<endl;
    // cout<<inicio_bm_bloques<<endl;

    // ---- 20 registros por linea
    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    // cout<<ruta<<endl;
    // cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    
    string ruta_creacion_txt = this->path;
    ofstream archivo;
    archivo.open(ruta_creacion_txt);
    if(archivo.fail())
    {
        cout<<"--->Error en el archivo reporte MBR"<<endl;
        archivo.close();
        fclose(discoLectura);
    }

    // ---- recorrer todo el bitmap
    fseek(discoLectura,inicio_bm_bloques,SEEK_SET);
    char lectura_bit;
    int contador_de_bitmap = 0;
    for(int bitmap = 0; bitmap<tota_bm_bloques; bitmap++)
    {
        if(contador_de_bitmap<20)
        {
            fread(&lectura_bit,sizeof(lectura_bit),1,discoLectura);
            archivo<<lectura_bit<<"-";
            contador_de_bitmap++;
        }
        else
        {
            archivo<<endl;
            fread(&lectura_bit,sizeof(lectura_bit),1,discoLectura);
            archivo<<lectura_bit<<"-";
            contador_de_bitmap=1;
        }
        inicio_bm_bloques++;
        fseek(discoLectura,inicio_bm_bloques,SEEK_SET);
    }
    archivo.close();

    fclose(discoLectura);
}



// ------- reporte del super bloque de la particion que se esta leendo
void reportes::reportSP(string rutaArchivo,int part_star)
{
    FILE *discoLectura;
    discoLectura = fopen(rutaArchivo.c_str(),"rb+");
    if(discoLectura==NULL)
    {
        fclose(discoLectura);
        exit(1);        
    }

    // ---- incio de la particion 
    superBloque SP;
    // cout<<"inicio de la particion es: "<<part_star<<endl;
    fseek(discoLectura,part_star,SEEK_SET);
    fread(&SP,sizeof(SP),1,discoLectura);

    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    // cout<<ruta<<endl;
    // cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    // --- creacion del archivo dot 
    string ruta_creacion_dot = ruta + "sp.dot";
    ofstream archivo;
    archivo.open(ruta_creacion_dot);
    if(archivo.fail())
    {
        cout<<"--->Error en el archivo reporte MBR"<<endl;
        archivo.close();
        fclose(discoLectura);
    }

    archivo<<"digraph{"<<endl;
    archivo<<" graph [pad=\"0.5\",nodesep=\"0.5\",ranksep=\"2\"]; "<<endl;
    archivo<<" node [shape=plain]"<<endl;
    archivo<<" rankdir=LR;"<<endl;
    // ----- escirtura de la informacion del MBR
    archivo<<"SP [label=<"<<endl;
    archivo<<"<table borde=\"0\" cellborde=\"1\" cellspacing=\"0\">"<<endl;
    // ---- informacion dle MBR leido
    archivo<<"<tr><td><i>Nombre</i></td><td><i>Valor</i></td></tr>"<<endl;
    archivo<<"<tr><td>s_inodes_count</td><td>"<<SP.s_inodes_count<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_blocks_count</td><td>"<<SP.s_blocks_count<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_free_blocks_count</td><td>"<<SP.s_free_blocks_count<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_free_inodes_count</td><td>"<<SP.s_free_inodes_count<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_mmtime</td><td>"<<SP.s_mtime<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_umtime</td><td>"<<SP.s_umtime<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_mnt_count</td><td>"<<SP.s_mnt_count<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_magic</td><td>"<<SP.s_magic<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_inode_size</td><td>"<<SP.s_inode_size<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_block_size</td><td>"<<SP.s_block_size<<"</td></tr>"<<endl; 
    archivo<<"<tr><td>s_first_ino</td><td>"<<SP.s_firts_ino<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_first_blo</td><td>"<<SP.s_first_blo<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_bm_inode_start</td><td>"<<SP.s_bm_inode_start<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_bm_block_start</td><td>"<<SP.s_bm_block_start<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_inode_star</td><td>"<<SP.s_inode_start<<"</td></tr>"<<endl;
    archivo<<"<tr><td>s_block_star</td><td>"<<SP.s_block_start<<"</td></tr>"<<endl;
    // ------------------------------
    archivo<<"</table>>];"<<endl;
    archivo<<"}"<<endl;
    archivo.close();

    // ----- cierre del disco
    fclose(discoLectura);

    // ---- conversion del archivo a tipo 
    if(this->nombreArchivoDot != "")
    {
        vector<string> nombre_extesion = FUN.split(nombreArchivoDot,'.');
        string tipo = tipoConversion(nombre_extesion[1]);
        string comando = "dot " + tipo + " " + ruta_creacion_dot + " -o " + this->path;
        // cout<<comando<<endl;
        system(comando.c_str());
    }
    else
    {
        cout<<"--->No se ha expecifica el tipo de extension"<<endl;
    }
}

// exec -path=/home/hector/prueba/calificacion.script
