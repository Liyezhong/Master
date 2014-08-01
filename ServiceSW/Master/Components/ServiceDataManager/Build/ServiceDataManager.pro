!include("ServiceDataManager.pri") {
    error("ServiceDataManager.pri not found")
}

QT += gui

TARGET = ServiceDataManager

INCLUDEPATH += ../../../../../../Platform/Common/Components \
               ../../../../../Shared/Master/Components \
               ../../../../../../Himalaya/HimalayaMain/Master/Include \
               ../../../../../../Himalaya/HimalayaMain/Master/Components

HEADERS +=  ../Include/*.h  \
    ../Include/TestCaseGuide.h \
    ../Include/TestCaseFactory.h \
    ../Include/TestCase.h \
    ../Include/SubModule.h \
    ../Include/ModuleDataList.h \
    ../Include/Module.h \
    ../Include/ModuleDataListVerifier.h \
    ../Include/InstrumentHistory.h \
    ../Include/ModuleDateListAdapter.h

SOURCES +=  ../Source/*.cpp \
    ../Source/TestCaseGuide.cpp \
    ../Source/TestCase.cpp \
    ../Source/TestCaseFactory.cpp \
    ../Source/InstrumentHistory.cpp \
    ../Source/ModuleDateListAdapter.cpp

