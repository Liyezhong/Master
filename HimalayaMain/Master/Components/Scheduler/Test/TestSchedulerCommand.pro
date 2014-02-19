!include("Scheduler.pri") {
    error("Scheduler.pri not found")
}

TARGET = utTestSchedulerCommand

SOURCES += ../Source/*.cpp \
			../Commands/Source/*.cpp \
			TestSchedulerCommand.cpp

HEADERS += ../Include/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Interface/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h \
            Mock/MockIDeviceProcessing.h

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler GPIOManager ExternalProcessController \
           NetworkComponents StateMachines)

HEADERS +=
QT += xml
QT += xmlpatterns
QT += network
