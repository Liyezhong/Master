!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler

#DEFINES += GOOGLE_MOCK

INCLUDEPATH += ../../../../Shared/Common/Components \

SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp

HEADERS += ../Include/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h

defined(GOOGLE_MOCK) {
HEADERS +=
          ../../../../Shared/Master/Components/DeviceControl/Test/Mock/*.h
}

RESOURCES +=

QT += xml

















