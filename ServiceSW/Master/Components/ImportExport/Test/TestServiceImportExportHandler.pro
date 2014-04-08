!include("../../../Test/HimalayaService.pri"):error("HimalayaService.pri not found")

TARGET = utTestServiceImportExportHandler
SOURCES += TestServiceImportExportHandler.cpp

INCLUDEPATH += ../../../../ \
 ../../../../../../../../Platform/Master/Components/

DEPENDPATH += ../../../../
	
UseLibs(Core ServiceMasterThreadController ServiceDataManager)
UsePlatformGUILibs(MainMenu Application)
UsePlatformLibs(Threads EventHandler Global DataManager NetCommands StateMachines NetworkComponents DeviceControl GPIOManager ImportExport DataLogging ExternalProcessController ExportController PasswordManager)
