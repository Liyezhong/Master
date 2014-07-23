!include("TestDiagnosticsManufacturing.pri"):error("TestDiagnosticsManufacturing.pri not found")

TARGET 	 = utTestDiagnosticsManufacturing
CONFIG   += console
SOURCES  += TestDiagnosticsManufacturing.cpp
TEMPLATE  = app


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

QT += testcase

UseLibs(Core ServiceDataManager DiagnosticsManufacturing Diagnostics)
UsePlatformGUILibs(KeyBoard MainMenu Application)
UsePlatformLibs(DataManager Global)
UsePlatformScroller(QtScroller)


