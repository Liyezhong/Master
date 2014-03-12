!include(ClientUser.pri) {
    error("ClientUser.pri not found")
}

TARGET = ClientUser         # functional unit name
HEADERS *= ../Include/*.h   # header file location
SOURCES *= ../Source/*.cpp  # source file location
