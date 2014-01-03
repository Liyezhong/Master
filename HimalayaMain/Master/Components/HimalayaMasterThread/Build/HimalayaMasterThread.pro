!include("HimalayaMasterThread.pri") {
    error("HimalayaMasterThread.pri not found")
}

QT += core gui

INCLUDEPATH += ../../../../../../Platform/Common/Components

TARGET = HimalayaMasterThread

HEADERS +=  ../Include/*.h \
            ../../ImportExport/Include/*.h \
    ../Include/ThreadIDs.h

SOURCES +=  ../Source/*.cpp \
            ../../ImportExport/Source/*.cpp
