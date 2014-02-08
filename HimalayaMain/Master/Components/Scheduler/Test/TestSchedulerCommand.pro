!include("Scheduler.pri") {
    error("Scheduler.pri not found")
}

TARGET = utTestSchedulerCommand

SOURCES += TestSchedulerCommand.cpp

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler GPIOManager ExternalProcessController \
           NetworkComponents StateMachines)

HEADERS +=
QT += xml
QT += xmlpatterns
QT += network
