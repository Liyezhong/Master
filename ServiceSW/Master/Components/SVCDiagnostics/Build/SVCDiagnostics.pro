#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T13:48:34
#
#-------------------------------------------------

!include(SVCDiagnostics.pri) {
    error("SVCDiagnostics.pri not found")
}

TARGET = SVCDiagnostics

HEADERS += ../Include/*.h      # header file location
SOURCES += ../Source/*.cpp     # source file location
FORMS   += ../Forms/*.ui       # form file location
RESOURCES *= ../Resource/SVCDiagnostics.qrc # resource file location


