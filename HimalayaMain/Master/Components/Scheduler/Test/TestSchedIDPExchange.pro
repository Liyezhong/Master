!include("Scheduler.pri") {
    error("Scheduler.pri not found")
}

TARGET = utTestSchedIDPExchange

SOURCES += ../Source/*.cpp \
			../Commands/Source/*.cpp \
                        TestSchedIDPExchange.cpp

HEADERS += ../Include/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Interface/*.h \
            ../../../../../../Platform/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h \
            Mock/MockIDeviceProcessing.h

UseDepLibs(Global DataLogging Threads NetCommands DeviceControl DataManager RemoteCareManager \
           RemoteCareController HeartBeatManager EventHandler GPIOManager ExternalProcessController \
           NetworkComponents StateMachines PasswordManager SWUpdateManager ExportController ImportExport)

HEADERS +=
QT += xml
QT += xmlpatterns
QT += network
