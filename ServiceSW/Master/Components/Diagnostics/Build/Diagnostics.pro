#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T13:48:34
#
#-------------------------------------------------

!include(Diagnostics.pri) {
    error("Diagnostics.pri not found")
}

TARGET = Diagnostics

HEADERS *= ../Include/*.h \    # header file location
    ../Include/Retort/*.h \
    ../Include/Oven/*.h \
    ../Include/RotaryValve/*.h \
    ../Include/LaSystem/*.h \
    ../Include/System/*.h

SOURCES *= ../Source/*.cpp\   # source file location
    ../Source/Retort/*.cpp \
    ../Source/Oven/*.cpp \
    ../Source/RotaryValve/*.cpp \
    ../Source/LaSystem/*.cpp \
    ../Source/System/*.cpp

FORMS   *= ../Forms/*.ui \     # form file location
    ../Forms/System/*.ui
