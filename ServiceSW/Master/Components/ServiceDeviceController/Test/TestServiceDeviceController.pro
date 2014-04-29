!include("../../../Test/HimalayaService.pri"):error("HimalayaService.pri not found")

TARGET = utTestServiceDeviceController
SOURCES += TestServiceDeviceController.cpp

INCLUDEPATH += ../../../../ \
 ../../../../../../../../Platform/Master/Components/

DEPENDPATH += ../../../../
	
UseLibs(Core ServiceMasterThreadController ServiceDataManager ServiceDeviceController)
#UseSharedLibs(ColoradoDataContainer)
UsePlatformLibs(Threads EventHandler Global DataManager NetCommands StateMachines NetworkComponents DeviceControl DeviceCommandProcessor GPIOManager DataLogging ExternalProcessController ExportController PasswordManager)
