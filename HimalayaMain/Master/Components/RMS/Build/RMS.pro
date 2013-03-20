!include("RMS.pri") {
    error("RMS.pri not found")
}

TARGET = RMS

HEADERS +=  ../Include/*.h

SOURCES +=  ../Source/*.cpp
