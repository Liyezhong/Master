!include("HimalayaMasterThread.pri") {
    error("HimalayaMasterThread.pri not found")
}

QT += core gui

INCLUDEPATH += ../../../../../../Platform/Common/Components

TARGET = HimalayaMasterThread

HEADERS +=  ../Include/*.h \
            ../../HimalayaDataManager/Include/*.h \
            ../../HimalayaDataManager/CommandInterface/Include/*.h \
            ../../HimalayaDataManager/Commands/Include/*.h \
            ../../ImportExport/Include/*.h

SOURCES +=  ../Source/*.cpp \
            ../../HimalayaDataManager/Source/*.cpp \
            ../../HimalayaDataManager/CommandInterface/Source/*.cpp \
            ../../HimalayaDataManager/Commands/Source/*.cpp \
            ../../ImportExport/Source/*.cpp
