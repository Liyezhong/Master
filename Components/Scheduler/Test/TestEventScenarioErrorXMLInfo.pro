!include("Scheduler.pri") {
    error("Scheduler.pri not found")
}
QT += xml
QT += xmlpatterns
QT += network

TARGET = utTestEventScenarioErrorXMLInfo

SOURCES += TestEventScenarioErrorXMLInfo.cpp

UseLibs(Scheduler)
