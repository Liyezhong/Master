!include(TestFramework.pri) {
    error("TestFramework.pri not found")
}

TARGET = TestFramework      # functional unit name
HEADERS *= ../Include/*.h   # header file location
SOURCES *= ../Source/*.cpp  # source file location
FORMS   *= ../Forms/*.ui    # form file location
