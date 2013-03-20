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
           ../../../../Platform/Master/Components/ImportExport/Build/ImportExport.pro \
           ../../../../Platform/Master/Components/NetworkComponents/Build/NetworkComponents.pro \
           ../../../../Platform/Master/Components/StateMachines/Build/StateMachines.pro \
           #../../../../Platform/Master/Components/AxedaController/Build/AxedaController.pro \
           ../../../../Platform/Master/Components/Threads/Build/Threads.pro \
           ../../../../Platform/Master/Components/NetCommands/Build/NetCommands.pro \
           ../../../../Platform/Master/Components/PasswordManager/Build/PasswordManager.pro \
           ../../../../Platform/Master/Components/SoftSwitchManager/Build/SoftSwitchManager.pro \
           ../../../Shared/Master/Components/HimalayaDataContainer/Build/HimalayaDataContainer.pro \
           ../../../Shared/Master/Components/RMSCommands/Build/RMSCommands.pro \
           ../Components/HimalayaMasterThread/Build/HimalayaMasterThread.pro \
           ../Components/HimalayaGuiController/Build/HimalayaGuiController.pro \
           ../Components/HimalayaErrorHandler/Build/HimalayaErrorHandler.pro \
           ../Components/Scheduler/Build/Scheduler.pro \
           ../Components/ExportController/Build/ExportController.pro \
            ../Components/RMS/Build/RMS.pro \
           ../Components/Main/Build/Main.pro

CONFIG += ordered

