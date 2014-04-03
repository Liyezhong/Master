!include("TestRsRvGetOriginalPositionAgain.pri") {
    error("TestRsRvGetOriginalPositionAgain.pri not found")
}

TARGET = utTestRsRvGetOriginalPositionAgain

SOURCES +=  TestRsRvGetOriginalPositionAgain.cpp

HEADERS += ../Include/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Interface/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h \

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler GPIOManager ExternalProcessController \
           NetworkComponents StateMachines PasswordManager SWUpdateManager ExportController ImportExport)
