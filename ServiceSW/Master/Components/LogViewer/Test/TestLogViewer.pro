!include("TestLogViewer.pri"):error("TestLogViewer.pri not found")

TARGET 	 = utTestLogViewer
CONFIG   += console
SOURCES  += TestLogViewer.cpp
TEMPLATE  = app


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \
               ../../../../../../Platform/ServiceSW/Components \

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Core ServiceDataManager LogViewer)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global)
UsePlatformServiceLibs(LogViewer)
UsePlatformScroller(QtScroller)

