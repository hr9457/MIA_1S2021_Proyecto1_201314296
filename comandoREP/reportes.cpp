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
    else if(reporte == "tree")
    {
        reportTree(rutaArchivo,part_star);
    }
    else if(reporte == "inode")
    {
        reportInode(rutaArchivo,part_star);
    }
    else if(reporte == "block")
    {
        reportBlock(rutaArchivo,part_star);
    }
    else if(reporte == "journaling")
    {
        reportJornali(rutaArchivo,part_star);
    }
    else if(reporte == "disk")
    {
        cout<<"reporte del disco"<<endl;
        reportDisk(rutaArchivo);
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
        return "-Tjpg";
    }
    else if(tipo == "pdf")
    {
        return "-Tpdf";
    }
    else
    {
        return "-Tpdf";
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



// ------ reporte del arbol que contiene la particion
void reportes::reportTree(string rutaArchivo,int part_star)
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

    // ------- datos
    int cantidad_inodos_usados = SP.s_firts_ino;
    int cantidad_bloque_usados = SP.s_first_blo;
    int inicio_de_inodos = SP.s_inode_start;
    int inicio_de_bloques = SP.s_block_start;


    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    // cout<<ruta<<endl;
    // cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    // --- creacion del archivo dot 
    string ruta_creacion_dot = ruta + "tree.dot";
    ofstream archivo;
    archivo.open(ruta_creacion_dot);
    if(archivo.fail())
    {
        cout<<"--->Error en el archivo reporte MBR"<<endl;
        archivo.close();
        fclose(discoLectura);
    }
    archivo<<"digraph{"<<endl;
    archivo<<" graph [pad=\"0.5\",nodesep=\"0.5\",ranksep=\"2\",splines=polyline]; "<<endl;
    archivo<<" node [shape=plain]"<<endl;
    archivo<<" rankdir=LR;"<<endl;

    // --------------------------------------------------------------------------------
    // ----------- INODOS
    // --- cilclo para recorrer los bloques 
    fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    for(int numero_inodo=0; numero_inodo<cantidad_inodos_usados; numero_inodo++)
    {        
        // ---- inodo por inodo
        inodo inodoLectura;
        fread(&inodoLectura,sizeof(inodoLectura),1,discoLectura);
        int tipo_bloque = inodoLectura.i_type;
        // ----------------------------------------------------------------------
        archivo<<"INODO"<<numero_inodo<<"[label=<"<<endl;
        archivo<<"<table>"<<endl;
        archivo<<"<tr><td port=\"E\" bgcolor=\"#535A92\"><i>INODO"<<numero_inodo<<"</i></td><td bgcolor=\"#535A92\"><i>Valor</i></td></tr>"<<endl;
        archivo<<"<tr><td>UID</td><td>"<<inodoLectura.i_uid<<"</td></tr>"<<endl;
        archivo<<"<tr><td>GID</td><td>"<<inodoLectura.i_gid<<"</td></tr>"<<endl;
        archivo<<"<tr><td>SIZE</td><td>"<<inodoLectura.i_size<<"</td></tr>"<<endl;
        archivo<<"<tr><td>ATIME</td><td>"<<inodoLectura.i_atime<<"</td></tr>"<<endl;
        archivo<<"<tr><td>CTIME</td><td>"<<inodoLectura.i_ctime<<"</td></tr>"<<endl;
        archivo<<"<tr><td>MTIME</td><td>"<<inodoLectura.i_mtime<<"</td></tr>"<<endl;
        archivo<<"<tr><td>TYPE</td><td>"<<tipo_bloque<<"</td></tr>"<<endl;
        archivo<<"<tr><td>PERM</td><td>"<<inodoLectura.i_perm<<"</td></tr>"<<endl;
        // ----------------------------------------------------------------------  
        // ------ ciclo para recorrido de los apuntadores directos de cada inodo   
        for(int pointerD=0; pointerD<12; pointerD++)
        {
            // ---------------------------------------------------------------------------------
            archivo<<"<tr><td>APD</td><td  port=\""<<pointerD<<"\" >"<<inodoLectura.i_block[pointerD]<<"</td></tr>"<<endl;
            // ---------------------------------------------------------------------------------                      
        }// for de apuntadores directos
        // ------ revision y posicionamiento al siguiente inodo
        inicio_de_inodos = inicio_de_inodos + sizeof(inodo);
        fseek(discoLectura,inicio_de_inodos,SEEK_SET);
        // ------------------------------
        archivo<<"</table>>];"<<endl;
        archivo<<endl;
    }
    

    // --------------------------------BLOQUES
    inicio_de_inodos = SP.s_inode_start;
    // --------------------------------------------------------------------------------
    // --- cilclo para recorrer los bloques 
    fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    for(int numero_inodo=0; numero_inodo<cantidad_inodos_usados; numero_inodo++)
    {        
        // ---- inodo por inodo
        inodo inodoLectura;
        fread(&inodoLectura,sizeof(inodoLectura),1,discoLectura);
        int tipo_bloque = inodoLectura.i_type;
        // ------ ciclo para recorrido de los apuntadores directos de cada inodo   
        for(int pointerD=0; pointerD<12; pointerD++)
        {
            // bloque a examinar
            int numero_bloque_inodo = inodoLectura.i_block[pointerD];
            if(numero_bloque_inodo != -1)
            {
                // tipo 0 = carpeta, tipo 1 = archivo
                if(tipo_bloque == '0')
                {
                    bloque_carpetas carpeta_lectura;
                    inicio_de_bloques = inicio_de_bloques + ( numero_bloque_inodo * sizeof(bloque_carpetas) );
                    fseek(discoLectura,inicio_de_bloques,SEEK_SET);
                    fread(&carpeta_lectura,sizeof(carpeta_lectura),1,discoLectura);
                    // ----------------------------------------------------------------
                    char encabezado = 'E';
                    archivo<<"BLOQUE"<<numero_bloque_inodo<<"[label=<"<<endl;
                    archivo<<"<table>"<<endl;
                    archivo<<"<tr><td port=\""<<encabezado<<"\" bgcolor=\"#54A759\"><i>BLOQUE"<<numero_bloque_inodo<<"</i></td><td bgcolor=\"#54A759\"><i>Valor</i></td></tr>"<<endl;
                    // ----------------------------------------------------------------
                    for(int x=0; x<4; x++)
                    {
                        archivo<<"<tr><td>"<<carpeta_lectura.b_content[x].b_name<<"</td><td port=\""<<x<<"\" >"<<carpeta_lectura.b_content[x].b_inodo<<"</td></tr>"<<endl;
                    }
                    // ------------------------------------------------------------------
                    archivo<<"</table>>];"<<endl;
                    archivo<<endl;
                    inicio_de_bloques = SP.s_block_start;
                }
                else if(tipo_bloque == '1')
                {
                    bloque_archivos archivo_lectura;
                    inicio_de_bloques = inicio_de_bloques + ( numero_bloque_inodo * sizeof(bloque_carpetas) );
                    fseek(discoLectura,inicio_de_bloques,SEEK_SET);
                    fread(&archivo_lectura,sizeof(archivo_lectura),1,discoLectura);
                    // cout<<archivo_lectura.b_content<<endl;
                    char encabezado = 'E';
                    // ----------------------------------------------------------------
                    archivo<<"BLOQUE"<<numero_bloque_inodo<<"[label=<"<<endl;
                    archivo<<"<table borde=\"0\" cellborde=\"1\" cellspacing=\"0\">"<<endl;
                    archivo<<"<tr><td port=\""<<encabezado<<"\" bgcolor=\"#D49B6A\"><i>Contenido</i></td></tr>"<<endl;
                    archivo<<"<tr><td>"<<archivo_lectura.b_content<<"</td></tr>"<<endl;
                    archivo<<"</table>>];"<<endl;
                    archivo<<endl;
                    // ----------------------------------------------------------------
                    inicio_de_bloques = SP.s_block_start;
                } 
            }                       
        }// for de apuntadores directos
        // ------ revision y posicionamiento al siguiente inodo
        inicio_de_inodos = inicio_de_inodos + sizeof(inodo);
        fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    }

    // ------------------------------------------------------------------------------------------------------
    // ------------------------------
    // APUNTADORES INODOS
    archivo<<endl;
    inicio_de_inodos = SP.s_inode_start;
    // --------------------------------------------------------------------------------
    // --- cilclo para recorrer los bloques 
    fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    for(int numero_inodo=0; numero_inodo<cantidad_inodos_usados; numero_inodo++)
    {        
        // ---- inodo por inodo
        inodo inodoLectura;
        fread(&inodoLectura,sizeof(inodoLectura),1,discoLectura);
        int tipo_bloque = inodoLectura.i_type;
        // ------ ciclo para recorrido de los apuntadores directos de cada inodo   
        for(int pointerD=0; pointerD<12; pointerD++)
        {
            // bloque a examinar
            int numero_bloque_inodo = inodoLectura.i_block[pointerD];
            if(numero_bloque_inodo != -1)
            {
                archivo<<"INODO"<<numero_inodo<<":"<<pointerD<<"-> BLOQUE"<<numero_bloque_inodo<<":E ;"<<endl;  
                if(tipo_bloque == '0')
                {
                    bloque_carpetas carpeta_lectura;
                    inicio_de_bloques = inicio_de_bloques + ( numero_bloque_inodo * sizeof(bloque_carpetas) );
                    fseek(discoLectura,inicio_de_bloques,SEEK_SET);
                    fread(&carpeta_lectura,sizeof(carpeta_lectura),1,discoLectura);
                    // ----------------------------------------------------------------
                    for(int x=0; x<4; x++)
                    {
                        if(strcmp(carpeta_lectura.b_content[x].b_name,"")==0 || strcmp(carpeta_lectura.b_content[x].b_name,".")==0
                        || strcmp(carpeta_lectura.b_content[x].b_name,"..")==0)
                        {                            
                        }
                        else
                        {
                            archivo<<"BLOQUE"<<numero_bloque_inodo<<":"<<x<<" -> INODO"<<carpeta_lectura.b_content[x].b_inodo<<":E;"<<endl;
                        }
                    }
                    inicio_de_bloques = SP.s_block_start;
                }              
            }                       
        }// for de apuntadores directos
        // ------ revision y posicionamiento al siguiente inodo
        inicio_de_inodos = inicio_de_inodos + sizeof(inodo);
        fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    }
    // ------------------------------------------------------------------------------------------------------
    // ------------------------------
    archivo<<endl;
    archivo<<"}"<<endl;
    archivo.close();
    // ------ cierre
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


// - reporte de los inodos
void reportes::reportInode(string rutaArchivo,int part_star)
{
    // --------------------------------------------
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

    // ----- datos
    int inicio_inodos = SP.s_inode_start;
    int cantidad_inodos = SP.s_firts_ino;

    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    // cout<<ruta<<endl;
    // cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    // --- creacion del archivo dot 
    string ruta_creacion_dot = ruta + "inode.dot";
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

    // -----------------------------------------------------------------------------------------------------
    // ------------ recorrido de todos los inodos
    fseek(discoLectura,inicio_inodos,SEEK_SET);
    for(int numero_inodo=0; numero_inodo<cantidad_inodos; numero_inodo++)
    {        
        // ---- inodo por inodo
        inodo inodoLectura;
        fread(&inodoLectura,sizeof(inodoLectura),1,discoLectura);
        int tipo_bloque = inodoLectura.i_type;
        // ----------------------------------------------------------------------
        archivo<<"INODO"<<numero_inodo<<"[label=<"<<endl;
        archivo<<"<table borde=\"0\" cellborde=\"1\" cellspacing=\"0\">"<<endl;
        archivo<<"<tr><td port=\"E\" ><i>Nombre</i></td><td><i>Valor</i></td></tr>"<<endl;
        archivo<<"<tr><td>UID</td><td>"<<inodoLectura.i_uid<<"</td></tr>"<<endl;
        archivo<<"<tr><td>GID</td><td>"<<inodoLectura.i_gid<<"</td></tr>"<<endl;
        archivo<<"<tr><td>SIZE</td><td>"<<inodoLectura.i_size<<"</td></tr>"<<endl;
        archivo<<"<tr><td>ATIME</td><td>"<<inodoLectura.i_atime<<"</td></tr>"<<endl;
        archivo<<"<tr><td>CTIME</td><td>"<<inodoLectura.i_ctime<<"</td></tr>"<<endl;
        archivo<<"<tr><td>MTIME</td><td>"<<inodoLectura.i_mtime<<"</td></tr>"<<endl;
        archivo<<"<tr><td>TYPE</td><td>"<<tipo_bloque<<"</td></tr>"<<endl;
        archivo<<"<tr><td>PERM</td><td>"<<inodoLectura.i_perm<<"</td></tr>"<<endl;
        // ---------------------------------------------------------------------- 
        // ------ ciclo para recorrido de los apuntadores directos de cada inodo   
        for(int pointerD=0; pointerD<15; pointerD++)
        {
            archivo<<"<tr><td>APD</td><td>"<<inodoLectura.i_block[pointerD]<<"</td></tr>"<<endl;                   
        }// for de apuntadores directos
        // ------ revision y posicionamiento al siguiente inodo
        inicio_inodos = inicio_inodos + sizeof(inodo);
        fseek(discoLectura,inicio_inodos,SEEK_SET);
        // ------------------------------
        archivo<<"</table>>];"<<endl;
        archivo<<endl;
    }
    // ------------------------------------------------------------------------------------------------------
    // ------------------------------
    // APUNTADORES
    inicio_inodos = SP.s_inode_start;
    fseek(discoLectura,inicio_inodos,SEEK_SET);
    for(int numero_inodo=0; numero_inodo<cantidad_inodos; numero_inodo++)
    {     
        // ----------------------------------------------------------------------
        // condicion final
        if(numero_inodo<cantidad_inodos-1)
        {
            archivo<<"INODO"<<numero_inodo<<" -> INODO"<<numero_inodo + 1<<endl;
            archivo<<endl;
        }        
    }
    // ------------------------------------------------------------------------------------------------------
    // ------------------------------
    archivo<<endl;
    archivo<<"}"<<endl;
    archivo.close();
    // ---------------------------------------------
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


// reporte de  los bloques
void reportes::reportBlock(string rutaArchivo,int part_star)
{    
    // --------------------------------------------
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
    // ------- datos
    int cantidad_inodos_usados = SP.s_firts_ino;
    int cantidad_bloque_usados = SP.s_first_blo;
    int inicio_de_inodos = SP.s_inode_start;
    int inicio_de_bloques = SP.s_block_start;

    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    // cout<<ruta<<endl;
    // cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    // --- creacion del archivo dot 
    string ruta_creacion_dot = ruta + "block.dot";
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

    // --------------------------------BLOQUES
    // --------------------------------------------------------------------------------
    // --- cilclo para recorrer los bloques 
    fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    for(int numero_inodo=0; numero_inodo<cantidad_inodos_usados; numero_inodo++)
    {        
        // ---- inodo por inodo
        inodo inodoLectura;
        fread(&inodoLectura,sizeof(inodoLectura),1,discoLectura);
        int tipo_bloque = inodoLectura.i_type;
        // ------ ciclo para recorrido de los apuntadores directos de cada inodo   
        for(int pointerD=0; pointerD<12; pointerD++)
        {
            // bloque a examinar
            int numero_bloque_inodo = inodoLectura.i_block[pointerD];
            if(numero_bloque_inodo != -1)
            {
                // tipo 0 = carpeta, tipo 1 = archivo
                if(tipo_bloque == 0)
                {
                    bloque_carpetas carpeta_lectura;
                    inicio_de_bloques = inicio_de_bloques + ( numero_bloque_inodo * sizeof(bloque_carpetas) );
                    fseek(discoLectura,inicio_de_bloques,SEEK_SET);
                    fread(&carpeta_lectura,sizeof(carpeta_lectura),1,discoLectura);
                    // ----------------------------------------------------------------
                    char encabezado = 'E';
                    archivo<<"BLOQUE"<<numero_bloque_inodo<<"[label=<"<<endl;
                    archivo<<"<table borde=\"0\" cellborde=\"1\" cellspacing=\"0\">"<<endl;
                    archivo<<"<tr><td port=\""<<encabezado<<"\" ><i>Nombre</i></td><td><i>Valor</i></td></tr>"<<endl;
                    // ----------------------------------------------------------------
                    for(int x=0; x<4; x++)
                    {
                        archivo<<"<tr><td>"<<carpeta_lectura.b_content[x].b_name<<"</td><td port=\""<<x<<"\" >"<<carpeta_lectura.b_content[x].b_inodo<<"</td></tr>"<<endl;
                    }
                    // ------------------------------------------------------------------
                    archivo<<"</table>>];"<<endl;
                    archivo<<endl;
                    inicio_de_bloques = SP.s_block_start;
                }
                else if(tipo_bloque == 1)
                {
                    bloque_archivos archivo_lectura;
                    inicio_de_bloques = inicio_de_bloques + ( numero_bloque_inodo * sizeof(bloque_carpetas) );
                    fseek(discoLectura,inicio_de_bloques,SEEK_SET);
                    fread(&archivo_lectura,sizeof(archivo_lectura),1,discoLectura);
                    // cout<<archivo_lectura.b_content<<endl;
                    char encabezado = 'E';
                    // ----------------------------------------------------------------
                    archivo<<"BLOQUE"<<numero_bloque_inodo<<"[label=<"<<endl;
                    archivo<<"<table borde=\"0\" cellborde=\"1\" cellspacing=\"0\">"<<endl;
                    archivo<<"<tr><td port=\""<<encabezado<<"\" ><i>Contenido</i></td></tr>"<<endl;
                    archivo<<"<tr><td>"<<archivo_lectura.b_content<<"</td></tr>"<<endl;
                    archivo<<"</table>>];"<<endl;
                    archivo<<endl;
                    // ----------------------------------------------------------------
                    inicio_de_bloques = SP.s_block_start;
                } 
            }                       
        }// for de apuntadores directos
        // ------ revision y posicionamiento al siguiente inodo
        inicio_de_inodos = inicio_de_inodos + sizeof(inodo);
        fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    }

    // --------------------------------APUNTADORES
    // --------------------------------------------------------------------------------
    // --- cilclo para recorrer los bloques 
    inicio_de_inodos = SP.s_inode_start;
    fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    for(int numero_inodo=0; numero_inodo<cantidad_inodos_usados; numero_inodo++)
    {        
        // ---- inodo por inodo
        inodo inodoLectura;
        fread(&inodoLectura,sizeof(inodoLectura),1,discoLectura);
        int tipo_bloque = inodoLectura.i_type;
        // ------ ciclo para recorrido de los apuntadores directos de cada inodo   
        for(int pointerD=0; pointerD<12; pointerD++)
        {
            // bloque a examinar
            int numero_bloque_inodo = inodoLectura.i_block[pointerD];
            if(numero_bloque_inodo != -1)
            {
                archivo<<"BLOQUE"<<numero_bloque_inodo<<" -> BLOQUE"<<numero_bloque_inodo + 1<<";"<<endl;
                archivo<<""<<endl;
            }                       
        }// for de apuntadores directos
        // ------ revision y posicionamiento al siguiente inodo
        inicio_de_inodos = inicio_de_inodos + sizeof(inodo);
        fseek(discoLectura,inicio_de_inodos,SEEK_SET);
    }
    // ------------------------------------------------------------------------------------------------------
    // ------------------------------
    archivo<<endl;
    archivo<<"}"<<endl;
    archivo.close();
    // ---------------------------------------------
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


// ----- reporte para el jornali
void reportes::reportJornali(string rutaArchivo,int part_star)
{
    // --------------------------------------------
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
    // ------- Datos
    int inicio_jornali = part_star + sizeof(superBloque);
    int total_inodos = SP.s_inodes_count;
    cout<<total_inodos<<endl;
    // cout<<inicio_jornali<<endl;

    // 
    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    // cout<<ruta<<endl;
    // cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    // --- creacion del archivo dot 
    string ruta_creacion_dot = ruta + "journaling.dot";
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
    
    // ---------------
    // jornali de lectura
    journal journal_lectura;
    fseek(discoLectura,inicio_jornali,SEEK_SET);

    archivo<<"JOURNALING [label=<"<<endl;
    archivo<<"<table>"<<endl;
    archivo<<"<tr><td>Operacon</td><td>Tipo</td><td>Nombre</td><td>Contenido</td><td>fecha</td><td>propietarios</td><td>permisos</td></tr>"<<endl;
    for(int a=0; a<total_inodos; a++)
    {
        fread(&journal_lectura,sizeof(journal),1,discoLectura);
        if(strcmp(journal_lectura.Journal_Tipo_Operacion,"")==0)
        {}
        else
        {            
            archivo<<"<tr><td>"<<journal_lectura.Journal_Tipo_Operacion<<"</td>";
            archivo<<"<td>"<<journal_lectura.Journal_tipo<<"</td>";
            archivo<<"<td>"<<journal_lectura.Journal_nombre<<"</td>";
            archivo<<"<td>"<<journal_lectura.Journal_contenido<<"</td>";
            archivo<<"<td>"<<journal_lectura.Journal_fecha<<"</td>";        
            archivo<<"<td>"<<journal_lectura.Journal_propietario<<"</td>";
            archivo<<"<td>"<<journal_lectura.Journal_permisos<<"</td></tr>";
            archivo<<endl;
            inicio_jornali = inicio_jornali + sizeof(journal);
            fseek(discoLectura,inicio_jornali,SEEK_SET);
        }
    }
    archivo<<"</table>>];"<<endl;
    // ------------------------------------------------------------------------------------------------------
    // ------------------------------
    archivo<<endl;
    archivo<<"}"<<endl;
    archivo.close();
    // ------- datos
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

// ------ reporte del disco
void reportes::reportDisk(string rutaArchivo)
{
    vector<partition3>copia_particiones;
    // --------------------------------------------
    FILE *discoLectura;
    discoLectura = fopen(rutaArchivo.c_str(),"rb+");
    if(discoLectura==NULL)
    {
        fclose(discoLectura);
        exit(1);        
    }
    // -------  lectura del mbr
    mbr MBR;
    fseek(discoLectura,0,SEEK_SET);
    fread(&MBR,sizeof(MBR),1,discoLectura);
    int tamanio_disco = MBR.mbr_tamanio;
    // -------- 
    fclose(discoLectura);
    // ---------
    // DATOS
    int tamanioDico = MBR.mbr_tamanio;
    char ajuste_disco = MBR.mbr_fit;
    
    // busqueda departiciones utilizadas
    for(int i=0;i<4;i++)
    {
        partition3 particion_encontrada;
        if(MBR.mbr_partitions[i].part_status == '1')
        {
            particion_encontrada.part_status = MBR.mbr_partitions[i].part_status;
            particion_encontrada.part_type = MBR.mbr_partitions[i].part_type;
            particion_encontrada.part_star = MBR.mbr_partitions[i].part_star;
            particion_encontrada.part_size = MBR.mbr_partitions[i].part_size;
            copia_particiones.push_back(particion_encontrada);
        }
    }
    // --------------------------------------
    // ordenamiento de particiones segun su inicio en el disco
    if(copia_particiones.empty()!=true)
    {
        for(int i=0; i<copia_particiones.size()-1; i++)
        {
            for(int j=i+1; j<copia_particiones.size(); j++)
            {
                if(copia_particiones[i].part_star > copia_particiones[j].part_star)
                {
                    partition3 aux = copia_particiones[i];
                    copia_particiones[i] = copia_particiones[j];
                    copia_particiones[j] = aux;
                }
            }
        }
    }
    // -------------------------------------
    // creacion de una lista con los espacios en blanco dentro del disco
    int inicio = sizeof(mbr);
    int inicio_particion;
    int size_disco;
    int espacio_libre;
    int inicio_anterior;
    // ------ recorrido
    for(int disco=0; disco<copia_particiones.size(); disco++)
    {
        inicio_particion = copia_particiones[disco].part_star;
        espacio_libre = inicio_particion - inicio;
        inicio_anterior = inicio;
        inicio = inicio_particion + copia_particiones[disco].part_star;
        if(espacio_libre>0)
        {
            // guardamos el espacio en blanco
            partition3 particion_libre;
            particion_libre.part_status = '0';
            particion_libre.part_type = 'h';
            particion_libre.part_star = inicio_anterior;
            particion_libre.part_size = espacio_libre;
            copia_particiones.push_back(particion_libre);
        }
    }
    // -------- condicion para ver si llego al final
    if(inicio < MBR.mbr_tamanio)
    {
        espacio_libre = MBR.mbr_tamanio - inicio;
        partition3 final_libre;
        final_libre.part_status = '0';
        final_libre.part_type = 'h';
        final_libre.part_star = inicio;
        final_libre.part_star = espacio_libre;
        copia_particiones.push_back(final_libre);
    }
    // ----------------------------------------------
    // ordenamiento final con los espacios en blanco
    if(copia_particiones.empty()!=true)
    {
        for(int i=0; i<copia_particiones.size()-1; i++)
        {
            for(int j=i+1; j<copia_particiones.size(); j++)
            {
                if(copia_particiones[i].part_star > copia_particiones[j].part_star)
                {
                    partition3 aux = copia_particiones[i];
                    copia_particiones[i] = copia_particiones[j];
                    copia_particiones[j] = aux;
                }
            }
        }
    }
    // -------------------------------------------------
    // impresion de particiones y espacios en blanco
    for(int espacio=0; espacio<copia_particiones.size(); espacio++)
    {
        cout<<copia_particiones[espacio].part_status<<"<--->";
        cout<<copia_particiones[espacio].part_type<<"<--->";
        cout<<copia_particiones[espacio].part_star<<"<--->";
        cout<<copia_particiones[espacio].part_size<<endl;
    }
    // ----------------------------------------------------
    // CREACION PARA EL REPORTE .DOT
    // 
    // ----------- estricutra del archivo dot del grafico
    string ruta = descomponerRuta(this->path);
    // cout<<ruta<<endl;
    // cout<<nombreArchivoDot<<endl;
    crearCarpetas(ruta);
    // --- creacion del archivo dot 
    string ruta_creacion_dot = ruta + "disk.dot";
    ofstream archivo;
    archivo.open(ruta_creacion_dot);
    if(archivo.fail())
    {
        cout<<"--->Error en el archivo reporte MBR"<<endl;
        archivo.close();
        fclose(discoLectura);
    }
    archivo<<"digraph DISCO{"<<endl;
    archivo<<"node [shape=record;]"<<endl;
    archivo<<"struct1 [ "<<endl;
    double porcentajeMBR = round(((double)sizeof(mbr) / (double)tamanio_disco)*100);
    archivo<<"label = \" MBR \\n"<<porcentajeMBR<<"% | "<<endl;
    // recorrido
    for(int espacio=0; espacio<copia_particiones.size(); espacio++)
    {
        if(copia_particiones[espacio].part_status == '1')
        {
            double porcentaje = round(((double)copia_particiones[espacio].part_size / (double)tamanio_disco)*100);
            // 
            if(copia_particiones[espacio].part_type == 'p')
            {
                archivo<<" PRIMARIA\\n"<<porcentaje<<"% | "<<endl;
            }
            else if(copia_particiones[espacio].part_type = 'e')
            {
                archivo<<" EXTENDIDA\\n"<<porcentaje<<"% | "<<endl;
            }
        }
        else if(copia_particiones[espacio].part_status == '0')
        {
            double porcentaje = round(((double)copia_particiones[espacio].part_size / (double)tamanio_disco )*100);
            archivo<<" LIBRE\\n"<<porcentaje<<"% | "<<endl;
        }
    }
    // 
    archivo<<" \" ];"<<endl;
    archivo<<"}"<<endl;


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