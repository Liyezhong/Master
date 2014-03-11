!include("GUIController.pri") {
    error("GUIController.pri not found")
}

QT += gui

INCLUDEPATH += ../../../../../../Platform/Common/Components

TARGET = GUIController

HEADERS +=  ../Include/*.h

SOURCES +=  ../Source/*.cpp
