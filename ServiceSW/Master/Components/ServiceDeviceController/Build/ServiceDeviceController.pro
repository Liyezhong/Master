!include("ServiceDeviceController.pri") {
    error("ServiceDeviceController.pri not found")
}

QT += gui

INCLUDEPATH += ../../../../../../Platform/Common/Components

TARGET = ServiceDeviceController

DEFINES += PRE_ALFA_TEST

HEADERS +=  ../Include/*.h \
            ../Include/Commands/*.h \
            ../Include/DeviceProcessor/*.h\
            ../Include/DeviceProcessor/Helper/*.h \
    ../Include/Commands/CmdAbortTest.h

SOURCES +=  ../Source/*.cpp \
            ../Source/Commands/*.cpp \
            ../Source/DeviceProcessor/*.cpp \
            ../Source/DeviceProcessor/Helper/*.cpp \
    ../Source/Commands/CmdLSensorDetectingTest.cpp


