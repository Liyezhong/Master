!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler
INCLUDEPATH += ../../../../Common/Components
SOURCES += ../Source/*.cpp
HEADERS += ../Include/*.h \
           ../../../../../../Platform/Master/Components/DeviceControl/Include/Interface/*.h \
           ../../../../../../Platform/Master/Components/DeviceControl/Include/Global/*.h


RESOURCES +=

QT += xml

















