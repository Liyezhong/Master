!include("TestSchedIDPExchange.pri") {
    error("TestSchedIDPExchange.pri not found")
}

TARGET = utTestSchedIDPExchange

SOURCES += ../Source/*.cpp \
			../Commands/Source/*.cpp \
                        TestSchedIDPExchange.cpp

INCLUDEPATH += ../../../../Shared/Common/Components/

HEADERS += ../Include/*.h \
            ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
            ../../../../Shared//Master/Components/DeviceControl/Include/Global/*.h \
            ../../../../Shared//Common/Components/FunctionModules/*.h \
                ../Commands/Include/*.h \
            Mock/MockIDeviceProcessing.h

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler ExternalProcessController \
           NetworkComponents StateMachines PasswordManager SWUpdateManager ExportController EncryptionDecryption)
LIBS += -ldl
