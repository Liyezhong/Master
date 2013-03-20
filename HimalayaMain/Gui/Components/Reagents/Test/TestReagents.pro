!include("TestReagents.pri"):error("TestReagents.pri not found")

TARGET = TestReagents
CONFIG   += console
SOURCES += TestReagents.cpp \

TEMPLATE = app


INCLUDEPATH += ../../ \
               ../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \
               ../../../../../HimalayaMain/Master/Include

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Reagents Core Programs  NetworkLayer)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global NetCommands NetworkComponents)
UseSharedLibs(HimalayaDataManager RMSCommands)
UsePlatformScroller(QtScroller)

