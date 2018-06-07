!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler

#DEFINES += GOOGLE_MOCK

INCLUDEPATH += ../../../../Shared/Common/Components \

SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp \
            ../Source/States/Instrument/*.cpp \
            ../Source/States/TPExecutor/*.cpp \
            ../Source/States/*.cpp \
            ../Source/Actions/*.cpp \
    ../Source/Actions/DrainAction.cpp \
    ../Source/Actions/SoakAction.cpp \
    ../Commands/Source/CmdALPurge.cpp
    #../Source/IAction.cpp


HEADERS += ../Include/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h \
            ../Include/States/Instrument/*.h \
            ../Include/States/TPExecutor/*.h \
            ../Include/States/*.h \
            ../Include/Actions/*.h \
    ../Include/Actions/ActionHelper.h \
    ../Include/Actions/PurgeAction.h \
    ../Include/Actions/DrainAction.h \
    ../Include/Actions/SoakAction.h \
    ../Commands/Include/CmdALPurge.h
    #../Include/IAction.h


defined(GOOGLE_MOCK) {
HEADERS +=
          ../../../../Shared/Master/Components/DeviceControl/Test/Mock/*.h
}

RESOURCES +=

QT += xml

















