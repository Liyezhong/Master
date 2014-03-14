!include("ServiceMasterThreadController.pri") {
    error("ServiceMasterThreadController.pri not found")
}

QT += gui

INCLUDEPATH += ../../../../../../Platform/Common/Components \
               ../../../../../Shared/Master/Components \
               ../../../../../../Himalaya/HimalayaMain/Master/Include \
               ../../../../../../Himalaya/HimalayaMain/Master/Components

TARGET = ServiceMasterThreadController

HEADERS +=  ../Include/*.h \
            ../../ServiceDataManager/CommandInterface/Include/*.h \
            ../../ServiceDataManager/Include/*.h \
            ../../ImportExport/Include/*.h

SOURCES +=  ../Source/*.cpp \
            ../../ServiceDataManager/CommandInterface/Source/*.cpp \
            ../../ServiceDataManager/Source/*.cpp \
            ../../ImportExport/Source/*.cpp

