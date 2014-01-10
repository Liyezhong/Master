!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler
INCLUDEPATH += ../../../../Common/Components
SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp \
    ../Source/RCReport.cpp

HEADERS += ../Include/*.h \
           ../../../../../../Platform/Master/Components/DeviceControl/Include/Interface/*.h \
           ../../../../../../Platform/Master/Components/DeviceControl/Include/Global/*.h \
             ../Commands/Include/*.h \
    ../Include/RCReport.h


RESOURCES +=

QT += xml

















