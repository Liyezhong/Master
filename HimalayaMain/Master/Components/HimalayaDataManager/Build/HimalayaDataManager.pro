!include("HimalayaDataManager.pri") {
    error("HimalayaDataManager.pri not found")
}

TARGET = HimalayaDataManager
INCLUDEPATH += ../../../../../../Platform/Common/Components

HEADERS +=  ../Include/*.h \
            ../CommandInterface/Include/*.h \
            ../Commands/Include/*.h
SOURCES +=  ../Source/*.cpp \
            ../CommandInterface/Source/*.cpp \
            ../Commands/Source/*.cpp
