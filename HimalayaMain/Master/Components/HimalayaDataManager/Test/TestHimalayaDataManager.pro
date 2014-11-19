!include("../../../Test/Himalaya.pri") {
    error("../../../Test/Himalaya.pri not found")
}

TARGET = utTestHimalayaDataManager
SOURCES += TestHimalayaDataManager.cpp

INCLUDEPATH += ../../../../ \
  ../../../../../../Platform/Master/Components/ \
  ../../../../../../Platform/Common/Components/ \
  ../../../../../Shared/Master/Components

DEPENDPATH += ../../../../


UseLibs(HimalayaDataManager HimalayaMasterThread Scheduler HimalayaGuiController HimalayaDataManager)
UseLibsShared(HimalayaDataContainer)
UseLibsPlatform(ExternalProcessController Threads HeartBeatManager ExportController SWUpdateManager EncryptionDecryption RemoteCareController RemoteCareManager DataLogging Global StateMachines DeviceControl EventHandler DataManager NetCommands NetworkComponents PasswordManager)
