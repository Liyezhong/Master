!include("RMSCommands.pri") {
    error("RMSCommands.pri not found")
}

TARGET = RMSCommands

HEADERS +=  ../Include/*.h
SOURCES +=  ../Source/*.cpp
