!include("TestHimalayaGuiController.pri") {
    error("TestHimalayaGuiController.pri not found")
}

TARGET = HimalayaGuiController

HEADERS +=  ../Include/*.h

SOURCES +=  ../Source/*.cpp
INCLUDEPATH += ../../../../../../Platform/Master/Components \
               ../../../../Gui/Test/Components/ \
               ../../../../../Shared/Master/Components/
