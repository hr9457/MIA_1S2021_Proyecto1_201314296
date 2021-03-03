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

// /*Estructura para el SUPER BLOQUE*/
// struct superBloque
// {
//     /* data */
//     int s_filesystem_tyep;
//     int s_inodes_count;
//     int s_blocks_count;
//     int s_free_blocks_count;
//     int s_free_inodes_count;
//     time_t s_mtime;
//     time_t s_umtime;
//     int s_mnt_count;
//     int s_magic;
//     int s_inode_size;
//     int s_block_size;
//     int s_firts_ino;
//     int s_first_blo;
//     int s_bm_inode_start;
//     int s_bm_block_start;
//     int s_inode_start;
//     int s_block_start;
// };

// /*Estructura para los inodos*/
// struct inodos
// {
//     /* data */
//     int i_uid;
//     int i_gid;
//     int i_size;
//     time_t i_atime;
//     time_t i_ctime;
//     time_t i_mtime;
//     int i_block;
//     char i_type;
//     int i_perm;    
// };


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



#endif // STRUCTS_H
