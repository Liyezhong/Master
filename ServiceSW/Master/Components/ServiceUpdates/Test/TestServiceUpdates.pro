!include("TestServiceUpdates.pri"):error("TestServiceUpdates.pri not found")

TARGET 	 = utTestServiceUpdates
CONFIG   += console
SOURCES  += TestServiceUpdates.cpp
TEMPLATE  = app


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Core ServiceDataManager ServiceUpdates)
UsePlatformServiceLibs(Settings)
UsePlatformGUILibs(KeyBoard MainMenu Application)
UsePlatformLibs(DataManager Global)
UsePlatformScroller(QtScroller)

