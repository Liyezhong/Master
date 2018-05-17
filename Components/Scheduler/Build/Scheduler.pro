!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler

#DEFINES += GOOGLE_MOCK

INCLUDEPATH += ../../../../Shared/Common/Components \

SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp \
           ../Source/States/Instrument/*.cpp \
            ../Source/States/*.cpp \


HEADERS += ../Include/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h \
            ../Include/States/Instrument/*.h \
            ../Include/States/*.h \

DEFINES += QSTATEMACHINE_DEBUG

defined(GOOGLE_MOCK) {
HEADERS +=
          ../../../../Shared/Master/Components/DeviceControl/Test/Mock/*.h
}

RESOURCES +=

QT += xml

















