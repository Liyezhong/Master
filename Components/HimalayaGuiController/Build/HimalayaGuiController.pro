!include("HimalayaGuiController.pri") {
    error("HimalayaGuiController.pri not found")
}

TARGET = HimalayaGuiController

HEADERS +=  ../Include/*.h

SOURCES +=  ../Source/*.cpp
