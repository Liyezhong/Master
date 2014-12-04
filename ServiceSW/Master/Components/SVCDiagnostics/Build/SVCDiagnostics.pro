#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T13:48:34
#
#-------------------------------------------------

!include(SVCDiagnostics.pri) {
    error("SVCDiagnostics.pri not found")
}

TARGET = SVCDiagnostics

HEADERS += ../Include/*.h \      # header file location
    ../Include/SVCValveStateInfoDlg.h
SOURCES += ../Source/*.cpp \     # source file location
    ../Source/SVCValveStateInfoDlg.cpp
FORMS   += ../Forms/*.ui \       # form file location
    ../Forms/SVCValveStateInfoDlg.ui
RESOURCES *= ../Resource/SVCDiagnostics.qrc # resource file location


