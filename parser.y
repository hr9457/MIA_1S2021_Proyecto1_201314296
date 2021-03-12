%{
#include "scanner.h"//se importa el header del analisis sintactico
#include <QString>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "qdebug.h"
#include <iostream>
#include <vector>
#include <dirent.h>
#include <stdio.h>
#include "Objetos/mkdisk.h"
#include "Objetos/rmdisk.h"
#include "Objetos/exec.h"
#include "CommandFDISK/fdisk.h"
#include "comandoMOUNT/mount.h"
#include "comandoUNMOUNT/unmount.h"
#include "comandoMKFS/mkfs.h"
#include "comandoLOGIN/login.h"
#include "comandoLOGOUT/logout.h"
#include "comandoMKGRP/mkgrp.h"
#include "comandoMKFILE/mkfile.h"
#include "comandoMKDIR/mkdir.h"
#include "comandoPAUSA/pausa.h"
#include "comandoREP/reportes.h"
#include "libreria/funciones.h"
#include "Estructuras/structs.h"
//#include "obmkdisk.h"
using namespace std;
extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON


/*vectores para los parametros de cada comando*/
vector<string> parametrosMkdisk;
string mkdiskParametros[4];
string rmdiskParametros;
string execParametro;
string fdiskParametros[8];
string mountParametros[2];
int indice = 0;
vector<montajeDisco> discos;
int numeroDisco = 0;
string identificadorUNMOUNT;
string mkfsParametros[3];
vector<usuarioConectado> usuarios; 
string loginParametros[3];
string mkgrpParametros[1];
string mkfileParametros[4];
string mkdirParametros[2];
string repParametros[4];

int yyerror(const char* mens)
{
std::cout << mens <<" "<<yytext<< std::endl;
return 0;
}
%}
//error-verbose si se especifica la opcion los errores sintacticos son especificados por BISON
%defines "parser.h"
%output "parser.cpp"
//%error-verbose
%locations
%union{
//se especifican los tipo de valores para los no terminales y lo terminales
//char TEXT [256];
//QString TEXT;
char TEXT[256];
/*objetos para cada comando*/
//class mkdisk *mkdisk;
}

//TERMINALES DE TIPO TEXT, SON STRINGS

%token<TEXT> tk_mkdisk;
%token<TEXT> tk_rmdisk;
%token<TEXT> tk_fdisk;
%token<TEXT> tk_exec;
%token<TEXT> tk_mount;
%token<TEXT> tk_unmount;
%token<TEXT> tk_mkfs;
%token<TEXT> tk_login;
%token<TEXT> tk_logout;
%token<TEXT> tk_mkgrp;
%token<TEXT> tk_mkfile;
%token<TEXT> tk_mkdir;
%token<TEXT> tk_pause;

%token<TEXT> tk_size;
%token<TEXT> tk_path;
%token<TEXT> tk_f;
%token<TEXT> tk_u;
%token<TEXT> tk_name;
%token<TEXT> tk_add;
%token<TEXT> tk_delete;
%token<TEXT> tk_type;
%token<TEXT> tk_id;
%token<TEXT> tk_fs;
%token<TEXT> tk_usr;
%token<TEXT> tk_pwd;
%token<TEXT> tk_r;
%token<TEXT> tk_p;
%token<TEXT> tk_rep;

%token<TEXT> guion;
%token<TEXT> igual;
%token<TEXT> interrogacion;
%token<TEXT> por;
%token<TEXT> punto;


%token<TEXT> entero;
%token<TEXT> cadena;
%token<TEXT> identificador;
%token<TEXT> tk_ruta;
%token<TEXT> comentario;

//%type<int> PARAMETROS_MKDISK;
//%type<mkdisk> MKDISK;


%start INICIO
%%

INICIO: LISTADO_COMANDOS ;


LISTADO_COMANDOS: LISTADO_COMANDOS COMANDO 
                | COMANDO ; 


COMANDO : MKDISK        {mkdisk disco; disco.crearDisco(mkdiskParametros);for(int i=0;i<sizeof(mkdiskParametros)/sizeof(mkdiskParametros[0]);i++){mkdiskParametros[i]="";}}
        | RMDSIK        {rmdisk eliminacion; eliminacion.eliminarDisco(rmdiskParametros);}
        | FDISK         {fdisk manejoParticiones;manejoParticiones.ejecutarFdisk(fdiskParametros);for(int i=0;i<sizeof(fdiskParametros)/sizeof(fdiskParametros[0]);i++){fdiskParametros[i]="";}} 
        | EXEC          {exec read; read.leerArchivo(execParametro);}
        | MOUNT         {mount montaje;montaje.montarDisco(mountParametros,discos,numeroDisco);for(int i=0;i<sizeof(mountParametros)/sizeof(mountParametros[0]);i++){mountParametros[i]="";}}
        | UNMOUNT       {unmount desmontaje;desmontaje.desmontarDisco(discos,identificadorUNMOUNT);} 
        | MKFS          {mkfs sisArchivos;sisArchivos.crearSistemaArchivos(mkfsParametros,discos);for(int i=0;i<sizeof(mkfsParametros)/sizeof(mkfsParametros[0]);i++){mkfsParametros[i]="";}}   
        | LOGIN         {login loginUsr;loginUsr.iniciarSession(usuarios,discos,loginParametros);for(int i=0;i<sizeof(loginParametros)/sizeof(loginParametros[0]);i++){loginParametros[i]="";}}
        | LOGOUT        {logout logoutUsuario;logoutUsuario.cerrarSession(usuarios);}
        | MKGRP         {mkgrp crear;crear.crearGrupo(usuarios,mkgrpParametros,discos);for(int i=0;i<sizeof(mkgrpParametros)/sizeof(mkgrpParametros[0]);i++){mkgrpParametros[i]="";}}
        | PAUSE         {pausa pausarSystem; pausarSystem.pausarSistema();}
        | REP           {reportes graficos; graficos.generarReporte(repParametros,usuarios,discos);for(int i=0;i<sizeof(repParametros)/sizeof(repParametros[0]);i++){repParametros[i]="";}}
        | MKFILE        {}
        | MKDIR         {}
        | COMENTARIO    {}        
        ;


COMENTARIO : comentario {} ;


MKDISK : tk_mkdisk LIST_PARAMETROS_MKDISK ;


LIST_PARAMETROS_MKDISK  : LIST_PARAMETROS_MKDISK PARAMETROS_MKDISK
                        | PARAMETROS_MKDISK ;


PARAMETROS_MKDISK : guion tk_size igual entero      {mkdiskParametros[0]=$4;}
                  | guion tk_path igual cadena      {mkdiskParametros[1]=$4;}
                  | guion tk_path igual tk_ruta     {mkdiskParametros[1]=$4;}
                  | guion tk_u igual identificador  {mkdiskParametros[2]=$4;}
                  | guion tk_f igual identificador  {mkdiskParametros[3]=$4;}
                  ;

RMDSIK : tk_rmdisk guion tk_path igual cadena  {rmdiskParametros=$5;}
       | tk_rmdisk guion tk_path igual tk_ruta {rmdiskParametros=$5;} 
       ;



FDISK : tk_fdisk LIST_PARAMETROS_FDISK {}
      ;

LIST_PARAMETROS_FDISK   : LIST_PARAMETROS_FDISK PARAMETROS_FDISK
                        | PARAMETROS_FDISK
                        ;

PARAMETROS_FDISK        : guion tk_path igual cadena            {fdiskParametros[0]=$4;}
                        | guion tk_path igual tk_ruta           {fdiskParametros[0]=$4;}
                        | guion tk_add igual entero             {fdiskParametros[1]=$4;}
                        | guion tk_delete igual identificador   {fdiskParametros[2]=$4;}
                        | guion tk_size igual entero            {fdiskParametros[3]=$4;}  
                        | guion tk_name igual identificador     {fdiskParametros[4]=$4;}
                        | guion tk_name igual cadena            {fdiskParametros[4]=$4;}
                        | guion tk_type igual identificador     {fdiskParametros[5]=$4;}
                        | guion tk_f igual identificador        {fdiskParametros[6]=$4;}
                        | guion tk_u igual identificador        {fdiskParametros[7]=$4;}                         
                        ;



MOUNT : tk_mount LIST_PARAMETROS_MOUNT
      ;

LIST_PARAMETROS_MOUNT   : LIST_PARAMETROS_MOUNT PARAMETROS_MOUNT
                        | PARAMETROS_MOUNT 
                        ;

PARAMETROS_MOUNT  :     guion tk_path igual tk_ruta         {mountParametros[0]=$4;}
                  |     guion tk_path igual cadena          {mountParametros[0]=$4;}
                  |     guion tk_name igual identificador   {mountParametros[1]=$4;}
                  |     guion tk_name igual cadena          {mountParametros[1]=$4;}
                  ;


UNMOUNT     :     tk_unmount guion tk_id igual identificador      {identificadorUNMOUNT=$5;}
            ;



MKFS  :     tk_mkfs LIST_PARAMETROS_MKFS
      ;

LIST_PARAMETROS_MKFS    :     LIST_PARAMETROS_MKFS PARAMETROS_MKFS
                        |     PARAMETROS_MKFS
                        ;

PARAMETROS_MKFS   :     guion tk_id igual identificador     {mkfsParametros[0]=$4;}
                  |     guion tk_type igual identificador   {mkfsParametros[1]=$4;}
                  |     guion tk_fs igual identificador     {mkfsParametros[2]=$4;}
                  ;




LOGIN :     tk_login LIST_PARAMETROS_LOGIN
      ;

LIST_PARAMETROS_LOGIN   : LIST_PARAMETROS_LOGIN PARAMETROS_LOGIN
                        | PARAMETROS_LOGIN
                        ;

PARAMETROS_LOGIN  :     guion tk_usr igual identificador    {loginParametros[0]=$4;}
                  |     guion tk_usr igual cadena           {loginParametros[0]=$4;}
                  |     guion tk_pwd igual entero           {loginParametros[1]=$4;}
                  |     guion tk_pwd igual cadena           {loginParametros[1]=$4;}
                  |     guion tk_id igual identificador     {loginParametros[2]=$4;}
                  ;



LOGOUT      :     tk_logout
            ;



MKGRP :      tk_mkgrp PARAMETROS_MKGRP     {}
      ;


PARAMETROS_MKGRP  :     guion tk_name igual identificador   {mkgrpParametros[0]=$4;}
                  |     guion tk_name igual cadena          {mkgrpParametros[0]=$4;}
                  ;






MKFILE      :     tk_mkfile LIST_PARAMETROS_MKFILE
            ;

LIST_PARAMETROS_MKFILE  :     LIST_PARAMETROS_MKFILE PARAMETROS_MKFILE
                        |     PARAMETROS_MKFILE
                        ;     

PARAMETROS_MKFILE :     guion tk_path igual tk_ruta   {mkfileParametros[0]=$4;}
                  |     guion tk_path igual cadena    {mkfileParametros[0]=$4;}
                  |     guion tk_r                    {mkfileParametros[1]="true";}
                  |     guion tk_size igual entero    {mkfileParametros[2]=$4;}
                  ;





MKDIR       :     tk_mkdir    LIST_PARAMETROS_MKDIR
            ;


LIST_PARAMETROS_MKDIR   :     LIST_PARAMETROS_MKDIR PARAMETROS_MKDIR
                        |     PARAMETROS_MKDIR
                        ;


PARAMETROS_MKDIR  :     guion tk_path igual tk_ruta   {mkdirParametros[0]=$4;}
                  |     guion tk_path igual cadena    {mkdirParametros[0]=$4;}
                  |     guion tk_p                    {mkdirParametros[1]="true";}
                  ;





PAUSE :     tk_pause
      ;



REP   :     tk_rep LIST_PARAMETROS_REP
      ;


LIST_PARAMETROS_REP     : LIST_PARAMETROS_REP PARAMETROS_REP
                        | PARAMETROS_REP
                        ;


PARAMETROS_REP    :     guion tk_name igual identificador   {repParametros[0]=$4;}
                  |     guion tk_path igual tk_ruta         {repParametros[1]=$4;}
                  |     guion tk_path igual cadena          {repParametros[1]=$4;}
                  |     guion tk_id igual identificador     {repParametros[2]=$4;}
                  |     guion tk_ruta igual identificador   {repParametros[3]=$4;}
                  |     guion tk_ruta igual cadena          {repParametros[3]=$4;}
                  ;




EXEC : tk_exec guion tk_path igual cadena     {execParametro=$5;}
     | tk_exec guion tk_path igual tk_ruta    {execParametro=$5;}
     ;
