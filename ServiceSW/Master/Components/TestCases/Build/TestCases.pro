!include(TestCases.pri) {
    error("TestCases.pri not found")
}

TARGET = TestCases          # functional unit name
HEADERS *= ../Include/*.h   # header file location
SOURCES *= ../Source/*.cpp  # source file location
