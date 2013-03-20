!include("../../../Test/Himalaya.pri") {
    error("../../../Test/Himalaya.pri not found")
}

TARGET = utTestRMSHandler
SOURCES += TestRMSHandler.cpp

INCLUDEPATH += ../../../../ \
  ../../../../../../../../Platform/Master/Components/ \
  ../../../../../Shared/Master/Components

DEPENDPATH += ../../../../

#UseLibs(RMS HimalayaMasterThread HimalayaDeviceCommandProcessor HimalayaGuiController HimalayaSepiaController)
#UseLibsPlatform(ExternalProcessController Threads DataLogging Global StateMachines DeviceControl EventHandler DataManager NetCommands SoftSwitchManager AxedaController NetworkComponents PasswordManager DeviceCommandProcessor)
#UseLibsShared(HimalayaDataManager RMSCommands)

