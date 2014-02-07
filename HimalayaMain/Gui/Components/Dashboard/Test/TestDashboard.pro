!include("TestDashboard.pri"):error("TestDashboard.pri not found")

TARGET = TestDashboard
CONFIG   += console
SOURCES += TestDashboard.cpp
TEMPLATE = app


INCLUDEPATH += ../../ \
               ../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \
               ../../../../../HimalayaMain/Master/Include

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Dashboard Core NetworkLayer)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global NetCommands NetworkComponents)
UseSharedLibs(HimalayaDataContainer)
UsePlatformScroller(QtScroller)


