# build all components recursive

TEMPLATE = subdirs

SHARED            =  ../../Shared

SUBDIRS += $${SHARED}/Master/Components/DataLogging/Build/DataLogging.pro
SUBDIRS += $${SHARED}/Master/Components/DataManager/Build/DataManager.pro
SUBDIRS += $${SHARED}/Master/Components/DeviceControl/Build/DeviceControl.pro
SUBDIRS += $${SHARED}/Master/Components/EventHandler/Build/EventHandler.pro

SUBDIRS += $${SHARED}/Master/Components/Global/Build/Global.pro
SUBDIRS += $${SHARED}/Master/Components/Global/Test/Global.pro

SUBDIRS += $${SHARED}/Master/Components/StateMachines/Build/StateMachines.pro
SUBDIRS += $${SHARED}/Master/Components/StateMachines/Test/StateMachines.pro

SUBDIRS += $${SHARED}/Master/Components/EncryptionDecryption/Build/EncryptionDecryption.pro
SUBDIRS += $${SHARED}/Master/Components/EncryptionDecryption/Test/EncryptionDecryption.pro

SUBDIRS += $${SHARED}/Master/Components/NetworkComponents/Build/NetworkComponents.pro
SUBDIRS += $${SHARED}/Master/Components/NetworkComponents/Test/NetworkComponentsTest.pro

#SUBDIRS += $${SHARED}/Master/Components/RemoteCareController/Build/RemoteCareController.pro
#SUBDIRS += $${SHARED}/Master/Components/RemoteCareManager/Build/RemoteCareManager.pro
SUBDIRS += $${SHARED}/Master/Components/Threads/Build/Threads.pro
SUBDIRS += $${SHARED}/Master/Components/NetCommands/Build/NetCommands.pro
SUBDIRS += $${SHARED}/Master/Components/PasswordManager/Build/PasswordManager.pro
SUBDIRS += $${SHARED}/Master/Components/PasswordManager/Test/PasswordManager.pro
SUBDIRS += $${SHARED}/Master/Components/GPIOManager/Build/GPIOManager.pro
SUBDIRS += $${SHARED}/Master/Components/ExportController/Build/ExportController.pro
SUBDIRS += $${SHARED}/Master/Components/HeartBeatManager/Build/HeartBeatManager.pro
SUBDIRS += $${SHARED}/Master/Components/HeartBeatManager/Test/TestHeartBeatThread.pro
SUBDIRS += $${SHARED}/Master/Components/SWUpdateManager/Build/SWUpdateManager.pro
SUBDIRS += ../../Master/Components/HimalayaDataManager/Build/HimalayaDataManager.pro
SUBDIRS += ../../Master/Components/HimalayaMasterThread/Build/HimalayaMasterThread.pro
SUBDIRS += ../../Master/Components/HimalayaGuiController/Build/HimalayaGuiController.pro
SUBDIRS += ../../Master/Components/Scheduler/Build/Scheduler.pro
SUBDIRS += $${SHARED}/Common/Components/HimalayaDataContainer/Build/HimalayaDataContainer.pro
SUBDIRS += $${SHARED}/Master/Components/ExternalProcessController/Build/ExternalProcessController.pro
SUBDIRS += ../../Master/Components/HimalayaGuiController/Test/TestHimalayaGuiController.pro
SUBDIRS += ../../Master/Components/HimalayaDataManager/Test/TestHimalayaDataManager.pro

SUBDIRS += ../../Master/Components/HimalayaMasterThread/Test/TestHimalayaMasterThread.pro
SUBDIRS += ../../Master/Components/SWUpdateTest/Test/TestSWUpdate.pro

#SUBDIRS += $${SHARED}/Common/Components/HimalayaDataContainer/Test/TestDataManager.pro
#SUBDIRS += $${SHARED}/Common/Components/HimalayaDataContainer/Containers/ReagentGroupColor/Test/TestReagentGroupColor.pro
#SUBDIRS += $${SHARED}/Common/Components/HimalayaDataContainer/Containers/ReagentGroups/Test/TestDataReagentGroupList.pro
#SUBDIRS += $${SHARED}/Common/Components/HimalayaDataContainer/Containers/ReagentStations/Test/TestDataStationList.pro
#SUBDIRS += $${SHARED}/Common/Components/HimalayaDataContainer/Containers/Reagents/Test/TestDataReagentList.pro
#SUBDIRS += $${SHARED}/Common/Components/HimalayaDataContainer/Containers/DashboardStations/Test/TestDashboardStationList.pro


SUBDIRS += $${SHARED}/Master/Components/ExternalProcessController/Test/ExternalProcessTest.pro

SUBDIRS += $${SHARED}/Master/Components/DeviceControl/Test/DeviceControl.pro
SUBDIRS += $${SHARED}/Master/Components/DataManager/Test/DataManager.pro
SUBDIRS += $${SHARED}/Master/Components/DataLogging/Test/DataLogging.pro
SUBDIRS += $${SHARED}/Master/Components/EventHandler/Test/EventHandler.pro
SUBDIRS += ../../Master/Components/Scheduler/Test/Scheduler.pro
#SUBDIRS += $${SHARED}/Master/Components/RemoteCareController/Test/TestRemoteCareController.pro
#SUBDIRS += $${SHARED}/Master/Components/RemoteCareManager/Test/TestRemoteCareManager.pro
SUBDIRS += $${SHARED}/Master/Components/ExportController/Test/TestExportController.pro



CONFIG += ordered
