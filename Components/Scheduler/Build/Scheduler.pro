!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler

INCLUDEPATH += ../../../../Shared/Common/Components
SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp

HEADERS += ../Include/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
../../../../Shared/Master/Components/DeviceControl/Test/Mock/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h

RESOURCES +=

QT += xml

















