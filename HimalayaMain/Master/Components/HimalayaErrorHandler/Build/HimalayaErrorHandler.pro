!include("HimalayaErrorHandler.pri") {
    error("HimalayaErrorHandler.pri not found")
}

TARGET = HimalayaErrorHandler

HEADERS +=  ../Include/Commands/*.h \
            ../Include/*.h

SOURCES +=  ../Source/Commands/*.cpp \
            ../Source/*.cpp


