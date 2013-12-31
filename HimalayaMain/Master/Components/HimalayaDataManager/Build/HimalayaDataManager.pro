!include("HimalayaDataManager.pri") {
    error("HimalayaDataManager.pri not found")
}

TARGET = HimalayaDataManager

HEADERS +=  ../Include/*.h \
            ../CommandInterface/Include/*.h \
            ../Commands/Include/*.h
SOURCES +=  ../Source/*.cpp \
            ../CommandInterface/Source/*.cpp \
            ../Commands/Source/*.cpp
