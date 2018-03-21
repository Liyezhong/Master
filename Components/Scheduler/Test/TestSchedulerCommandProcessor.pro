!include("TestSchedulerCommandProcessor.pri") {
    error("TestSchedulerCommandProcessor.pri not found")
}

TARGET = utTestSchedulerCommandProcessor

SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp
SOURCES +=  TestSchedulerCommandProcessor.cpp\
../../../../Shared/Master/Components/DeviceControl/Test/Mock/MockDeviceControl.h\


INCLUDEPATH += ../../../../Shared/Common/Components/

HEADERS += ../Include/*.h \
            ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
            ../../../../Shared/Master/Components/DeviceControl/Include/Global/*.h \
            ../../../../Shared/Common/Components/FunctionModules/*.h \
            ../Commands/Include/*.h \            \
            ../../../../Shared/Master/Components/DeviceControl/Test/Mock/MockDeviceControl.h\

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler ExternalProcessController \
           NetworkComponents StateMachines PasswordManager SWUpdateManager ExportController EncryptionDecryption)
LIBS += -ldl
