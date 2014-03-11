#-------------------------------------------------
#
# Project created by QtCreator 2013-01-16T16:50:09
#
#-------------------------------------------------

!include(LogViewer.pri) {
    error("LogViewer.pri not found")
}

TARGET = LogViewer

HEADERS *= ../Include/*.h   # header file location

SOURCES *= ../Source/*.cpp  # source file location

FORMS *= ../Forms/*.ui
