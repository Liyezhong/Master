!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler

INCLUDEPATH += ../../../../Shared/Common/Components \

SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp \
           ../Source/States/Instrument/*.cpp \
            ../Source/States/*.cpp \


HEADERS += ../Include/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
../../../../Shared/Master/Components/DeviceControl/Test/Mock/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h \
            ../Include/States/Instrument/*.h \
            ../Include/States/*.h \

DEFINES += QSTATEMACHINE_DEBUG

RESOURCES +=

QT += xml

















