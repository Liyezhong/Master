!include("TestProgramStatusInfor.pri") {
    error("TestProgramStatusInfor.pri not found")
}

TARGET = utProgramStatusInfor

INCLUDEPATH += ../../../../Shared/Common/Components/

SOURCES +=  TestProgramStatusInfor.cpp \
            ../Source/ProgramStatusInfor.cpp

HEADERS +=  ../Include/ProgramStatusInfor.h

LIBS += -ldl
