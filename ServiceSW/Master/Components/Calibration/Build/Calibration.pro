!include(Calibration.pri) {
    error("Calibration.pri not found")
}

TARGET = Calibration        # functional unit name
HEADERS *= ../Include/*.h   # header file location
SOURCES *= ../Source/*.cpp  # source file location
FORMS   *= ../Forms/*.ui    # form file location
