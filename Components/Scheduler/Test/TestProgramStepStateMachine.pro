!include("TestProgramStepStateMachine.pri") {
    error("TestProgramStepStateMachine.pri not found")
}

TARGET = utTestProgramStepStateMachine

SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp
SOURCES +=  TestProgramStepStateMachine.cpp

INCLUDEPATH += ../../../../../../Platform/Common/Components/

HEADERS += ../Include/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Interface/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Global/*.h \
            ../../../../../../Platform/Common/Components/FunctionModules/*.h \
            ../Commands/Include/*.h \

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler GPIOManager ExternalProcessController \
           NetworkComponents StateMachines PasswordManager SWUpdateManager ExportController ImportExport)

LIBS += -ldl
