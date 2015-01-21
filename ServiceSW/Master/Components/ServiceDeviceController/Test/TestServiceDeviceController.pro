!include("../../../Test/HimalayaService.pri"):error("HimalayaService.pri not found")

TARGET = utTestServiceDeviceController
SOURCES += TestServiceDeviceController.cpp

INCLUDEPATH += ../../../../ \
 ../../../../../../../../Platform/Master/Components/

DEPENDPATH += ../../../../
	
UseLibs(Core Diagnostics ServiceMasterThreadController ServiceDataManager ServiceDeviceController)
UsePlatformLibs(Threads EventHandler Global DataManager NetCommands StateMachines NetworkComponents DeviceControl DeviceCommandProcessor GPIOManager DataLogging ExternalProcessController ExportController PasswordManager)
LIBS            += -ldl
