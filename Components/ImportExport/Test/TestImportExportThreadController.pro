!include("../../../Test/Himalaya.pri"):error("Himalaya.pri not found")

TARGET = utTestImportExportThreadController
SOURCES += TestImportExportThreadController.cpp

INCLUDEPATH += ../../../../ \
 ../../../../../../../../Platform/Master/Components/

DEPENDPATH += ../../../../
	
UseLibs(HimalayaMasterThread)
UseLibsPlatform(ExternalProcessController Threads DataLogging Global NetworkComponents StateMachines EventHandler)
