# build all components recursive

TEMPLATE = subdirs

######## normal build process ########
#
# Make sure your Main.pro is in the last line to ensure correct linking!
#
SUBDIRS  = \
           # Nested subdirs project files fail with qmake -r
           # so list all components separately.
           \
           # ../../../../Platform/Master/Build/Platform.pro \
           ../../../../Platform/Master/Components/DataLogging/Build/DataLogging.pro \
           ../../../../Platform/Master/Components/DataManager/Build/DataManager.pro \
           ../../../../Platform/Master/Components/DeviceControl/Build/DeviceControl.pro \
           ../../../../Platform/Master/Components/EventHandler/Build/EventHandler.pro \
           ../../../../Platform/Master/Components/ExternalProcessController/Build/ExternalProcessController.pro \
           ../../../../Platform/Master/Components/Global/Build/Global.pro \
          ../../../../Platform/Master/Components/EncryptionDecryption/Build/EncryptionDecryption.pro \
           ../../../../Platform/Master/Components/NetworkComponents/Build/NetworkComponents.pro \
           ../../../../Platform/Master/Components/StateMachines/Build/StateMachines.pro \
           ../../../../Platform/Master/Components/RemoteCareController/Build/RemoteCareController.pro \
           ../../../../Platform/Master/Components/RemoteCareManager/Build/RemoteCareManager.pro \
           ../../../../Platform/Master/Components/Threads/Build/Threads.pro \
           ../../../../Platform/Master/Components/NetCommands/Build/NetCommands.pro \
           ../../../../Platform/Master/Components/PasswordManager/Build/PasswordManager.pro \
           ../../../../Platform/Master/Components/GPIOManager/Build/GPIOManager.pro \
           ../../../../Platform/Master/Components/ExportController/Build/ExportController.pro \
           ../../../../Platform/Master/Components/HeartBeatManager/Build/HeartBeatManager.pro \
            ../../../../Platform/Master/Components/SWUpdateManager/Build/SWUpdateManager.pro \
           ../../../Shared/Master/Components/HimalayaDataContainer/Build/HimalayaDataContainer.pro \
           ../Components/HimalayaMasterThread/Build/HimalayaMasterThread.pro \
           ../Components/HimalayaGuiController/Build/HimalayaGuiController.pro \
            ../Components/HimalayaDataManager/Build/HimalayaDataManager.pro \
#           ../Components/HimalayaErrorHandler/Build/HimalayaErrorHandler.pro \
           ../Components/Scheduler/Build/Scheduler.pro \
           ../Components/Main/Build/Main.pro

CONFIG += ordered

