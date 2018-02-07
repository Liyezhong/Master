!include("Scheduler.pri") {
    error("Scheduler.pri not found")
}
QT += xml
QT += xmlpatterns
QT += network

TARGET = utTestErrHandlingMap

SOURCES += TestErrHandlingMap.cpp

UseLibs(Scheduler)
UseDepLibs(EventHandler Global  DataManager DataLogging)
