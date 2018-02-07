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
           ../../Shared/Master/Components/DataLogging/Build/DataLogging.pro \
           ../../Shared/Master/Components/DataManager/Build/DataManager.pro \
           ../../Shared/Master/Components/DeviceControl/Build/DeviceControl.pro \
           ../../Shared/Master/Components/EventHandler/Build/EventHandler.pro \
           ../../Shared/Master/Components/ExternalProcessController/Build/ExternalProcessController.pro \
           ../../Shared/Master/Components/Global/Build/Global.pro \
           ../../Shared/Master/Components/EncryptionDecryption/Build/EncryptionDecryption.pro \
           ../../Shared/Master/Components/NetworkComponents/Build/NetworkComponents.pro \
           ../../Shared/Master/Components/StateMachines/Build/StateMachines.pro \
           ../../Shared/Master/Components/RemoteCareController/Build/RemoteCareController.pro \
           ../../Shared/Master/Components/RemoteCareManager/Build/RemoteCareManager.pro \
           ../../Shared/Master/Components/Threads/Build/Threads.pro \
           ../../Shared/Master/Components/NetCommands/Build/NetCommands.pro \
           ../../Shared/Master/Components/PasswordManager/Build/PasswordManager.pro \
           ../../Shared/Master/Components/ExportController/Build/ExportController.pro \
           ../../Shared/Master/Components/HeartBeatManager/Build/HeartBeatManager.pro \
           ../../Shared/Master/Components/SWUpdateManager/Build/SWUpdateManager.pro \
           ../../Shared/Common/Components/HimalayaDataContainer/Build/HimalayaDataContainer.pro \
           ../Components/HimalayaMasterThread/Build/HimalayaMasterThread.pro \
           ../Components/HimalayaGuiController/Build/HimalayaGuiController.pro \
            ../Components/HimalayaDataManager/Build/HimalayaDataManager.pro \
           ../Components/Scheduler/Build/Scheduler.pro \
           ../Components/Main/Build/Main.pro

CONFIG += ordered

