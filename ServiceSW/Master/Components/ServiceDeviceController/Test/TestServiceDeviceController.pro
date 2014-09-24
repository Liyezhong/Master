!include("../../../Test/HimalayaService.pri"):error("HimalayaService.pri not found")

DEFINES += GOOGLE_MOCK
TARGET = utTestServiceDeviceController
SOURCES += TestServiceDeviceController.cpp

INCLUDEPATH += ../../../../ \
 ../../../../../../../../Platform/Master/Components/

#Add google-mock related headers into our project
INCLUDEPATH += /usr/local/include

DEPENDPATH += ../../../../

###################################
### stuff for adding libraries ####
###################################
#Add google-mock related libraries into our project
PRE_TARGETDEPS  += /usr/local/lib/gmock/libgmock.a
LIBS            += /usr/local/lib/gmock/libgmock.a
PRE_TARGETDEPS  += /usr/local/lib/gtest/libgtest.a
LIBS            += /usr/local/lib/gtest/libgtest.a


	
UseLibs(Core ServiceMasterThreadController ServiceDataManager ServiceDeviceController)
UsePlatformLibs(Threads EventHandler Global DataManager NetCommands StateMachines NetworkComponents DeviceControl DeviceCommandProcessor GPIOManager DataLogging ExternalProcessController ExportController PasswordManager)
LIBS            += -ldl
