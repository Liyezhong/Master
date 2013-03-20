!include("Threads.pri") {
    error("Threads.pri not found")
}



TARGET = Threads
INCLUDEPATH += ../../../../Common/Components \
               ../../../../../../../Platform/Master/Components/ cd

HEADERS +=  ../Include/*.h

SOURCES +=  ../Source/*.cpp
