!include("TestRcReheating.pri") {
    error("TestRcReheating.pri not found")
}

TARGET = utTestRcReheating

SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp
SOURCES +=  TestRcReheating.cpp

INCLUDEPATH += ../../../../../../Platform/Common/Components/

HEADERS += ../Include/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Interface/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Global/*.h \
            ../../../../../../Platform/Common/Components/FunctionModules/*.h \
            ../Commands/Include/*.h \
            Mock/MockIDeviceProcessing.h

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler GPIOManager ExternalProcessController \
           NetworkComponents StateMachines PasswordManager SWUpdateManager ExportController EncryptionDecryption)

LIBS += -ldl
