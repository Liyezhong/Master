!include("TestDiagnostics.pri"):error("TestDiagnostics.pri not found")

TARGET 	 = utTestDiagnostics
CONFIG   += console
SOURCES  += TestDiagnostics.cpp
TEMPLATE  = app


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Core ServiceDataManager Diagnostics)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global)
# UseSharedLibs(HimalayaDataContainer)
UsePlatformScroller(QtScroller)

