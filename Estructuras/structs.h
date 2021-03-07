#ifndef STRUCTS_H
#define STRUCTS_H

// #include <ctime>
#include <iostream>
#include <string>

/*Estructura para una particion*/
struct partition
{
    char part_status;
    char part_type;
    char part_fit;
    int part_star;
    int part_size;
    char part_name[16];
};

/*Estructura para la copia de una particion*/
struct partition2
{
    char part_status;
    char part_type;
    char part_fit;
    int part_star;
    int part_size;
    char part_name[16];
};

/*Estructura para un MBR(Master Boot Record)*/
struct mbr
{
    int mbr_tamanio;
    char mbr_fecha_creacion[16];
    int mbr_disk_signature;
    char mbr_fit;
    partition mbr_partitions[4];
};

/*Estructura para el JOURNAL*/
struct journal
{
    char Journal_Tipo_Operacion[10] ; //mkfs,mkfile,...etc
    char Journal_tipo; //Carpeta(0), Archivo(1).
    char Journal_nombre [200]; //..../home....../home/chepe........../home/chepe/datos.txt
    char Journal_contenido [200]; //....este es el contenido de mi archivo de texto.....-
    char Journal_fecha[16]; // 18/06/2019 10:30
    char Journal_propietario [10]; //Quien creó el archivo o carpeta
    int Journal_permisos; //777...664
};

/*Estructura para el SUPER BLOQUE*/
struct superBloque
{
    /* data */
    int s_filesystem_tyep;//numero del sistema de archivos
    int s_inodes_count;// numero total de indos
    int s_blocks_count;// guarda el numero total de bloques
    int s_free_blocks_count;// contiene el numero de bloques libres
    int s_free_inodes_count;// contiene el numero de inodos libres
    char s_mtime[16];//ultima fecha el disco fue montada
    char s_umtime[16];// ultima fecha en el que el disco fue desmontado
    int s_mnt_count;// indica cuantas veces se ha montado el sistema
    int s_magic;// valor identifica al sistema de archivs 0xEF53
    int s_inode_size;// tamanio del inodo
    int s_block_size;// tamano del bloque
    int s_firts_ino;// primer indodo libre
    int s_first_blo;// primer bloque libre
    int s_bm_inode_start;// guardara el inicio del bloque de bitmap
    int s_bm_block_start;// guardara el inicio del bloque de bitmap de bloques
    int s_inode_start;// guardara el inicio de la tabla de inodos
    int s_block_start;// guaradara el inicio de la tabla de bloques
};

/*Estructura para los inodos*/
struct inodo
{
    /* data */
    int i_uid;// uid del usiario propietario 
    int i_gid;// gid del grupo al que pertene al archivo o carpeta
    int i_size;// tamanio del archivo en bytes
    char i_atime[16];// utltima fecha en que se leyo el inodo sin modificarlos
    char i_ctime[16];// fecha en la que se creo el inodo
    char i_mtime[16];// ultima fecha en la que se modifico el inodo
    int i_block[15];// array 12 registros 
    char i_type;// indica si e sarchivo o carpeta 1=archivo, 0=carpeta
    int i_perm;// guardara los permisos del archivo o carpeta    
};

/*Estructura para el content*/
struct content
{
    /* data */
    char b_name[12];
    int b_inodo;
};

/*Estructura del bloque de carpetas*/
struct bloque_carpetas
{
    /* data */
    content b_content[4];
};


/*Estrura del bloque de archivos*/
struct bloque_archivos
{
    /* data */
    char b_content[64];
};


/* Estructura par las particiones en blaco */
struct blackPartition
{
    /* data */
    int part_star;
    int part_size;
};


/* Estructura para identificar el particion montada */
struct partitionMontada
{
    /* data */
    char status;
    int part_star;
    int sizeDisco;
    char identificador[200];
    char nombreParticion[200];
};


/* Estructura para montaje de un disco */
struct montajeDisco
{
    /* data */
    char rutaArchivo[600];
    int numeroDisco;
    partitionMontada particiones[4];
};


/* Estructura para iniciar session  */
struct usuarioConectado
{
    /* data */
    char usuario[20];
    char password[20];
    char identificador[50];
};




#endif // STRUCTS_H
