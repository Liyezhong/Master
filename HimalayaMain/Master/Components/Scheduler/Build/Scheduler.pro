!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler
SOURCES += ../Source/*.cpp
HEADERS += ../Include/*.h*

RESOURCES +=

QT += xml

















