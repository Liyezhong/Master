!include("TestDiagnostics.pri"):error("TestDiagnostics.pri not found")

TARGET 	 = utTestDiagnostics
CONFIG   += console
SOURCES  += TestDiagnostics.cpp
TEMPLATE  = app

#HEADERS = ../Include/*


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

#QMAKE_CXXFLAGS += -std=c++0x

UseLibs(Core ServiceDataManager Diagnostics DiagnosticsManufacturing)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global)
# UseSharedLibs(HimalayaDataContainer)
UsePlatformScroller(QtScroller)

LIBS += -ldl

