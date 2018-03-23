!include("../../../Test/Skyline.pri") {
    error("../../../Test/Skyline.pri not found")
}

TARGET = utTestHimalayaGuiController
SOURCES += TestHimalayaGuiController.cpp

INCLUDEPATH += ../../ \
  ../../../../Shared/Master/Components\
../../../../Shared/Common/Components

DEPENDPATH += ../../../../


UseLibs(HimalayaDataManager HimalayaMasterThread Scheduler HimalayaGuiController)
UseLibsShared(HimalayaDataContainer)
UseLibsPlatform(ExternalProcessController Threads HeartBeatManager ExportController SWUpdateManager EncryptionDecryption RemoteCareController RemoteCareManager DataLogging Global StateMachines DeviceControl EventHandler DataManager NetCommands NetworkComponents PasswordManager)
LIBS += -lrt -ldl
