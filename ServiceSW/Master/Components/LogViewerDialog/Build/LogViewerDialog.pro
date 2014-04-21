#-------------------------------------------------
#
# Project created by QtCreator 2013-01-16T16:50:09
# Rename LogViewer to LogViewerDialog by Sunny 2014-04-14
#
#-------------------------------------------------

!include(LogViewerDialog.pri) {
    error("LogViewerDialog.pri not found")
}

TARGET = LogViewerDialog

HEADERS *= ../Include/*.h   # header file location

SOURCES *= ../Source/*.cpp  # source file location

FORMS *= ../Forms/*.ui
