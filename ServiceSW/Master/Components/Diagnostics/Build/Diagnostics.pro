#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T13:48:34
#
#-------------------------------------------------

!include(Diagnostics.pri) {
    error("Diagnostics.pri not found")
}

INCLUDEPATH += ../../../../../../Platform/Common/Components

DEFINES += PRE_ALFA_TEST

HEADERS *= ../Include/*.h \    # header file location
    ../Include/Retort/*.h \
    ../Include/Oven/*.h \
    ../Include/RotaryValve/*.h \
    ../Include/LaSystem/*.h \
    ../Include/System/*.h \
    ../Include/MainControl/*.h \
    ../Include/InitialSystem/*.h \
    ../Include/Display.h \
    ../Include/BasicColor/BasicColorTestDlg.h \
    ../Include/MainControl.h \
    ../Include/ServiceDeviceProcess/ServiceDeviceProcess.h \
    ../Include/InitialSystem.h

SOURCES *= ../Source/*.cpp\   # source file location
    ../Source/Retort/*.cpp \
    ../Source/Oven/*.cpp \
    ../Source/RotaryValve/*.cpp \
    ../Source/LaSystem/*.cpp \
    ../Source/System/*.cpp \
    ../Source/MainControl/*.cpp \
    ../Source/InitialSystem/*.cpp \
    ../Source/BasicColor/BasicColorTestDlg.cpp \
    ../Source/MainControl.cpp \
    ../Source/ServiceDeviceProcess/ServiceDeviceProcess.cpp \
    ../Source/InitialSystem.cpp \
    ../Source/TestBase.cpp

FORMS   *= ../Forms/*.ui \     # form file location
    ../Forms/System/*.ui \
    ../Forms/BasicColor/BasicColorTestDlg.ui \
    ../Forms/Display.ui \
    ../Forms/MainControl.ui \
    ../Forms/InitialSystem.ui
