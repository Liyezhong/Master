!include("TestPrograms.pri"):error("TestPrograms.pri not found")

TARGET = TestPrograms
CONFIG   += console
SOURCES += TestPrograms.cpp
TEMPLATE = app


INCLUDEPATH += ../../ \
               ../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \
               ../../../../../HimalayaMain/Master/Include

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Programs Core NetworkLayer)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global NetCommands NetworkComponents)
UsePlatformScroller(QtScroller)

