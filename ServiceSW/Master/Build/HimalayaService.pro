# build all components recursive

TEMPLATE = subdirs

######## normal build process ########
#
# Make sure your Main.pro is in the last line to ensure correct linking!
#

# path
HIMALAYA                = $$system(cd ../../../../Himalaya;pwd)
SHARED                  = $${HIMALAYA}/Shared
PLATFORM                = $$system(cd ../../../../Platform;pwd)
PLATFORM_SERVICE_SW     = $${PLATFORM}/ServiceSW
HIMALAYA_SERVICE_SW     = $${HIMALAYA}/ServiceSW

SUBDIRS = $${PLATFORM}/Master/Components/Global/Build/Global.pro \
          $${PLATFORM}/Master/Components/NetworkComponents/Build/NetworkComponents.pro \
          $${PLATFORM}/Master/Components/Threads/Build/Threads.pro \
          $${PLATFORM}/Master/Components/DataLogging/Build/DataLogging.pro \
          $${PLATFORM}/Master/Components/DataManager/Build/DataManager.pro \
          $${PLATFORM}/Master/Components/EventHandler/Build/EventHandler.pro \
          $${PLATFORM}/Master/Components/NetCommands/Build/NetCommands.pro \
          $${PLATFORM}/Gui/Components/Application/Build/Application.pro \
          $${PLATFORM}/Gui/Components/kineticscroller/qtscroller.pro \
          $${PLATFORM}/Gui/Components/MainMenu/Build/MainMenu.pro \
          $${PLATFORM}/Master/Components/DeviceControl/Build/DeviceControl.pro \
          $${PLATFORM}/Master/Components/DeviceCommandProcessor/Build/DeviceCommandProcessor.pro \
          $${PLATFORM}/Gui/Components/KeyBoard/Build/KeyBoard.pro \
          $${PLATFORM}/Master/Components/GPIOManager/Build/GPIOManager.pro \
          $${PLATFORM}/Master/Components/EncryptionDecryption/Build/EncryptionDecryption.pro \
          $${PLATFORM}/Master/Components/ExternalProcessController/Build/ExternalProcessController.pro \
          $${PLATFORM}/Master/Components/PasswordManager/Build/PasswordManager.pro \
          $${PLATFORM}/Master/Components/StateMachines/Build/StateMachines.pro \
          $${PLATFORM}/Master/Components/ExportController/Build/ExportController.pro \
          $${PLATFORM_SERVICE_SW}/Components/LogViewer/Build/LogViewer.pro \
          $${PLATFORM_SERVICE_SW}/Components/Settings/Build/Settings.pro \
          $${PLATFORM_SERVICE_SW}/Components/ServiceKeyValidator/Build/ServiceKeyValidator.pro \
          $${PLATFORM_SERVICE_SW}/Components/IENetworkClient/Build/IENetworkClient.pro \
          $${PLATFORM_SERVICE_SW}/Components/ServiceImportExport/Build/ServiceImportExport.pro \
          $${SHARED}/Gui/Components/Images/Build/Images.pro \
          $${SHARED}/Master/Components/HimalayaDataContainer/Build/HimalayaDataContainer.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/TestCases/Build/TestCases.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/ServiceWidget/Build/ServiceWidget.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/SVCDiagnostics/Build/SVCDiagnostics.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/SVCScreenLock/Build/SVCScreenLock.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/Calibration/Build/Calibration.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/SystemTracking/Build/SystemTracking.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/LogViewerDialog/Build/LogViewerDialog.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/Diagnostics/Build/Diagnostics.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/DiagnosticsManufacturing/Build/DiagnosticsManufacturing.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/ServiceUpdates/Build/ServiceUpdates.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/Core/Build/Core.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/ServiceDeviceController/Build/ServiceDeviceController.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/ServiceMasterThreadController/Build/ServiceMasterThreadController.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/ServiceDataManager/Build/ServiceDataManager.pro \
          $${HIMALAYA_SERVICE_SW}/Master/Components/Main/Build/Main.pro


CONFIG += ordered

TRANSLATIONS = $${HIMALAYA_SERVICE_SW}/Master/Components/Main/Build/TranslationsService/HimalayaService_en.ts \
               $${HIMALAYA_SERVICE_SW}/Master/Components/Main/Build/TranslationsService/HimalayaService_zh.ts
               #../Components/Main/Build/TranslationsService/Himalaya_ja.ts
#TRANSLATIONS = ../Components/Main/Build/TranslationsService/HimalayaService_en.pm \
#               ../Components/Main/Build/TranslationsService/HimalayaService_zh.pm

