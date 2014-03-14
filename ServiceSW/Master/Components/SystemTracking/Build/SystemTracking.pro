#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T14:19:28
#
#-------------------------------------------------

!include(SystemTracking.pri) {
    error("SystemTracking.pri not found")
}

TARGET = SystemTracking

HEADERS *= ../Include/*.h   # header file location

SOURCES *= ../Source/*.cpp  # source file location

FORMS *= ../Forms/*.ui

