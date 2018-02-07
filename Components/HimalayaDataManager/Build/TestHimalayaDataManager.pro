!include("TestHimalayaDataManager.pri") {
    error("TestHimalayaDataManager.pri not found")
}

TARGET = HimalayaDataManager

HEADERS +=  ../Include/*.h

SOURCES +=  ../Source/*.cpp
