!include("TestSVCDiagnostics.pri"):error("TestSVCDiagnostics.pri not found")

TARGET 	 = utTestSVCDiagnostics
CONFIG   += console
SOURCES  += TestSVCDiagnostics.cpp
TEMPLATE  = app


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \
               ../Build/

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Core ServiceDataManager SVCDiagnostics Diagnostics)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global)
# UseSharedLibs(HimalayaDataContainer)
UsePlatformScroller(QtScroller)

