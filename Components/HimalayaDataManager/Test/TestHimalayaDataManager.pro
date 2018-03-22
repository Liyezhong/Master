!include("../../../Test/Skyline.pri") {
    error("../../../Test/Skyline.pri not found")
}

TARGET = utTestHimalayaDataManager
SOURCES += TestHimalayaDataManager.cpp

INCLUDEPATH += ../../../../ \
  ../../../../../Shared/Master/Components

DEPENDPATH += ../../../../


UseLibs(HimalayaDataManager HimalayaMasterThread Scheduler HimalayaGuiController HimalayaDataManager)
UseLibsShared(HimalayaDataContainer)
UseLibsPlatform(ExternalProcessController Threads HeartBeatManager ExportController SWUpdateManager EncryptionDecryption RemoteCareController RemoteCareManager DataLogging Global StateMachines DeviceControl EventHandler DataManager NetCommands NetworkComponents PasswordManager)
LIBS += -lrt
