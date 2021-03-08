QT += core
QT -= gui

CONFIG += c++11

TARGET = MIA_1S2021_Proyecto1_201314296
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    scanner.cpp \
    parser.cpp \
    Objetos/mkdisk.cpp \
    Objetos/rmdisk.cpp \
    libreria/funciones.cpp \
    Objetos/exec.cpp \
    CommandFDISK/fdisk.cpp \
    comandoMOUNT/mount.cpp \
    comandoUNMOUNT/unmount.cpp \
    comandoMKFS/mkfs.cpp \
    comandoLOGIN/login.cpp \
    comandoLOGOUT/logout.cpp \
    comandoMKGRP/mkgrp.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DISTFILES += \
    lexico.l \
    parse.y

HEADERS += \
    scanner.h \
    parser.h \
    Objetos/mkdisk.h \
    parser.y \
    parser.y \
    parser.y \
    Estructuras/structs.h \
    Objetos/rmdisk.h \
    libreria/funciones.h \
    Objetos/exec.h \
    CommandFDISK/fdisk.h \
    comandoMOUNT/mount.h \
    comandoUNMOUNT/unmount.h \
    comandoMKFS/mkfs.h \
    comandoLOGIN/login.h \
    comandoLOGOUT/logout.h \
    comandoMKGRP/mkgrp.h
