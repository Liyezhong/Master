!include("TestSystemTracking.pri"):error("TestSystemTracking.pri not found")

TARGET 	 = utTestSystemTracking
CONFIG   += console
SOURCES  += TestSystemTracking.cpp
TEMPLATE  = app


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Core ServiceDataManager SystemTracking)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global)
# UseSharedLibs(HimalayaDataContainer)
UsePlatformScroller(QtScroller)

