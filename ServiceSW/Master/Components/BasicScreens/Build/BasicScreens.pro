!include(BasicScreens.pri) {
    error("BasicScreens.pri not found")
}

TARGET = BasicScreens       # functional unit name
HEADERS *= ../Include/*.h \   # header file location

SOURCES *= ../Source/*.cpp  # source file location
FORMS   *= ../Forms/*.ui    # form file location
