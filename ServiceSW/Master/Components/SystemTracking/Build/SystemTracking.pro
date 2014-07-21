#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T14:19:28
#
#-------------------------------------------------

!include(SystemTracking.pri) {
    error("SystemTracking.pri not found")
}

TARGET = SystemTracking

HEADERS *= ../Include/*.h \   # header file location
    ../Include/CurrentConfigParameterDlg.h \
    ../Include/ViewHistoryDiffDlg.h

SOURCES *= ../Source/*.cpp \  # source file location
    ../Source/CurrentConfigParameterDlg.cpp

FORMS *= ../Forms/*.ui \
    ../Forms/CurrentConfigurationDlg.ui \
    ../Forms/CurrentConfigParameterDlg.ui \
    ../Forms/ViewHistoryDiffDlg.ui

