# build all components recursive

TEMPLATE = subdirs

HIMALAYA                =  ../../../../Himalaya
HIMALAYA_MAIN     = $${HIMALAYA}/HimalayaMain
SHARED                    =  $${HIMALAYA}/Shared
PLATFORM               =  ../../../../Platform

SUBDIRS += $${PLATFORM}/Master/Components/DataLogging/Build/DataLogging.pro
SUBDIRS += $${PLATFORM}/Master/Components/DataManager/Build/DataManager.pro
SUBDIRS += $${PLATFORM}/Master/Components/DeviceControl/Build/DeviceControl.pro
SUBDIRS += $${PLATFORM}/Master/Components/EventHandler/Build/EventHandler.pro
SUBDIRS += $${PLATFORM}/Master/Components/ExternalProcessController/Build/ExternalProcessController.pro
SUBDIRS += $${PLATFORM}/Master/Components/Global/Build/Global.pro
SUBDIRS += $${PLATFORM}/Master/Components/ImportExport/Build/ImportExport.pro
SUBDIRS += $${PLATFORM}/Master/Components/NetworkComponents/Build/NetworkComponents.pro
SUBDIRS += $${PLATFORM}/Master/Components/StateMachines/Build/StateMachines.pro
SUBDIRS += $${PLATFORM}/Master/Components/RemoteCareController/Build/RemoteCareController.pro
SUBDIRS += $${PLATFORM}/Master/Components/RemoteCareManager/Build/RemoteCareManager.pro
SUBDIRS += $${PLATFORM}/Master/Components/Threads/Build/Threads.pro
SUBDIRS += $${PLATFORM}/Master/Components/NetCommands/Build/NetCommands.pro
SUBDIRS += $${PLATFORM}/Master/Components/PasswordManager/Build/PasswordManager.pro
SUBDIRS += $${PLATFORM}/Master/Components/GPIOManager/Build/GPIOManager.pro
SUBDIRS += $${PLATFORM}/Master/Components/ExportController/Build/ExportController.pro
SUBDIRS += $${PLATFORM}/Master/Components/HeartBeatManager/Build/HeartBeatManager.pro
SUBDIRS += $${PLATFORM}/Master/Components/SWUpdateManager/Build/SWUpdateManager.pro
SUBDIRS += $${PLATFORM}/Master/Components/DeviceControl/Test/DeviceControl.pro
SUBDIRS += $${PLATFORM}/Master/Components/DataManager/Test/DataManager.pro
SUBDIRS += $${PLATFORM}/Master/Components/DataLogging/Test/DataLogging.pro
SUBDIRS += $${PLATFORM}/Master/Components/EventHandler/Test/EventHandler.pro

SUBDIRS += $${HIMALAYA_MAIN}/Master/Components/HimalayaMasterThread/Build/HimalayaMasterThread.pro
SUBDIRS += $${HIMALAYA_MAIN}/Master/Components/HimalayaGuiController/Build/HimalayaGuiController.pro
SUBDIRS += $${HIMALAYA_MAIN}/Master/Components/HimalayaDataManager/Build/HimalayaDataManager.pro
SUBDIRS += $${HIMALAYA_MAIN}/Master/Components/Scheduler/Build/Scheduler.pro
#SUBDIRS += $${HIMALAYA_MAIN}/Master/Components/Scheduler/Test/Scheduler.pro

SUBDIRS += $${SHARED}/Master/Components/HimalayaDataContainer/Build/HimalayaDataContainer.pro

CONFIG += ordered
