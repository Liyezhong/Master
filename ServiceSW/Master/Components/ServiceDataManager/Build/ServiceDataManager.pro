!include("ServiceDataManager.pri") {
    error("ServiceDataManager.pri not found")
}

QT += gui

TARGET = ServiceDataManager

INCLUDEPATH += ../../../../../../Platform/Common/Components \
               ../../../../../Shared/Master/Components \
               ../../../../../../Himalaya/HimalayaMain/Master/Include \
               ../../../../../../Himalaya/HimalayaMain/Master/Components

HEADERS +=  ../Include/*.h

SOURCES +=  ../Source/*.cpp

