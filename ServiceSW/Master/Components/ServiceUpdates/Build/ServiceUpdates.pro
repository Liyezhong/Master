
!include(ServiceUpdates.pri) {
    error("ServiceUpdates.pri not found")
}

TARGET = ServiceUpdates

HEADERS *= ../Include/*.h \      # header file location
    ../Include/NetworkWidget.h

SOURCES *= ../Source/*.cpp \    # source file location
    ../Source/NetworkWidget.cpp

FORMS   *= ../Forms/*.ui \       # form file location
    ../Forms/NetworkWidget.ui
