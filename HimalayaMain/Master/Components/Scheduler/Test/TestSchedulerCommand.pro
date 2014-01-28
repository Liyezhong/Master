!include("Scheduler.pri") {
    error("Scheduler.pri not found")
}

TARGET = utTestSchedulerCommand

SOURCES += TestSchedulerCommand.cpp

UseDepLibs(Global DataLogging)

HEADERS +=
QT += xml
