#-------------------------------------------------
#
# Project created by QtCreator 2018-04-28T14:43:22
#
#-------------------------------------------------

!include("Scheduler.pri") {
    error("Scheduler.pri not found")
}
#QT       += testlib

#QT       -= gui

QT += xml
QT += xmlpatterns
QT += network

TARGET = utInstrumentManager
#CONFIG   += console
#CONFIG   -= app_bundle

#TEMPLATE = app

DEFINES -= GOOGLE_MOCK
DEFINES += QSTATEMACHINE_DEBUG
CONFIG +=QSTATEMACHINE_DEBUG

SOURCES += ../Source/*.cpp
SOURCES += ../Commands/Source/*.cpp
SOURCES += TestInstrumentManager.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"



#TARGET = utTestSchedulerMainThreadController

#SOURCES += ../Source/*.cpp
#SOURCES += ../Commands/Source/*.cpp
#SOURCES += TestSchedulerMainThreadController.cpp

INCLUDEPATH += ../../../../Shared/Common/Components/
INCLUDEPATH += ../..
INCLUDEPATH += ../Include
INCLUDEPATH += ../../../../Common/Components/ \
               ../../../../Shared/ExternalPackages/xsde/xsde-3.2.0-x86_64-linux-gnu/libxsde/

HEADERS += ../Include/*.h \
            ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
            ../../../../Shared/Master/Components/DeviceControl/Include/Global/*.h \
            ../../../../Shared/Common/Components/FunctionModules/*.h \
            ../Commands/Include/*.h \
            Mock/MockIDeviceProcessing.h

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler ExternalProcessController\
           NetworkComponents StateMachines PasswordManager SWUpdateManager ExportController EncryptionDecryption)
LIBS += ../../../../Shared/ExternalPackages/xsde/xsde-3.2.0-x86_64-linux-gnu/libxsde/xsde/libxsde.a
LIBS += -ldl
