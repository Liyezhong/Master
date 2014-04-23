#-------------------------------------------------
#
# Project created by QtCreator 2014-04-21T16:01:21
#
#-------------------------------------------------
!include("TestLogViewerDialog.pri"):error("TestLogViewerDialog.pri not found")

TARGET 	 = TestLogViewerDialog
CONFIG   += console
SOURCES  += TestLogViewerDialog.cpp
TEMPLATE  = app


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \
               ../../../../../../Platform/ServiceSW/Components \
               ../Include/

HEADERS += ../Include/*.h
HEADERS += ../../../../../../Platform/ServiceSW/Components/


CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Core ServiceDataManager LogViewerDialog)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global)
UsePlatformServiceLibs(LogViewer)
UsePlatformScroller(QtScroller)

