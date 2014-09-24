!include("../../../Test/HimalayaService.pri"):error("HimalayaService.pri not found")

TARGET = utTestServiceDataManager
SOURCES += TestServiceDataManager.cpp

INCLUDEPATH += ../../../../ \
 ../../../../../../../../Platform/Master/Components/

DEPENDPATH += ../../../../
	
UseLibs(Core ServiceMasterThreadController ServiceDataManager)
#UseSharedLibs(HimalayaDataContainer)
UsePlatformLibs(Threads EventHandler Global DataManager NetCommands StateMachines NetworkComponents DeviceControl GPIOManager  DataLogging ExternalProcessController ExportController PasswordManager)
