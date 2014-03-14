
!include(ServiceUpdates.pri) {
    error("ServiceUpdates.pri not found")
}

TARGET = ServiceUpdates

HEADERS *= ../Include/*.h     # header file location

SOURCES *= ../Source/*.cpp    # source file location

FORMS   *= ../Forms/*.ui      # form file location
