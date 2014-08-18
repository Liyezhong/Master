
!include(ServiceUpdates.pri) {
    error("ServiceUpdates.pri not found")
}

TARGET = ServiceUpdates

HEADERS *= ../Include/*.h \      # header file location
    ../Include/NetworkWidget.h \
    ../Include/System.h

SOURCES *= ../Source/*.cpp \    # source file location
    ../Source/NetworkWidget.cpp \
    ../Source/System.cpp

FORMS   *= ../Forms/*.ui \       # form file location
    ../Forms/NetworkWidget.ui \
    ../Forms/ServiceSystem.ui
