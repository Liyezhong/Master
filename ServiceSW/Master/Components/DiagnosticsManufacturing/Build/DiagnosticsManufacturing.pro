#-------------------------------------------------
#
# Project created by QtCreator 2013-04-23T13:48:34
#
#-------------------------------------------------

!include(DiagnosticsManufacturing.pri) {
    error("DiagnosticsManufacturing.pri not found")
}

TARGET = DiagnosticsManufacturing

HEADERS *= ../Include/*.h \          # header file location
    ../Include/TestReportGeneration.h \    

SOURCES *= ../Source/*.cpp        # source file location

FORMS += ../Forms/*.ui

RESOURCES *= ../Resource/*.qrc   # resource file location


