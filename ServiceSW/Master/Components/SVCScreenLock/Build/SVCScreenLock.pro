#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T13:48:34
#
#-------------------------------------------------

!include(SVCScreenLock.pri) {
    error("SVCScreenLock.pri not found")
}

TARGET = SVCScreenLock

HEADERS += ../Include/*.h      # header file location
SOURCES += ../Source/*.cpp     # source file location
FORMS   += ../Forms/*.ui \
    ../Forms/SVCScreenLockWidget.ui      # form file location
RESOURCES *= ../Resource/SVCScreenLock.qrc # resource file location


