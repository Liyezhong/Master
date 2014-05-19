# build all components recursive

TEMPLATE = subdirs

######## normal build process ########
#
# Make sure your Main.pro is in the last line to ensure correct linking!
#
SUBDIRS = ../../../../Platform/Master/Components/Global/Build/Global.pro \
          ../../../../Platform/Master/Components/NetworkComponents/Build/NetworkComponents.pro \
          ../../../../Platform/Master/Components/Threads/Build/Threads.pro \
          ../../../../Platform/Master/Components/DataLogging/Build/DataLogging.pro \
          ../../../../Platform/Master/Components/DataManager/Build/DataManager.pro \
          ../../../../Platform/Master/Components/EventHandler/Build/EventHandler.pro \
          ../../../../Platform/Master/Components/NetCommands/Build/NetCommands.pro \
          ../../../../Platform/Gui/Components/Application/Build/Application.pro \
          ../../../../Platform/Gui/Components/kineticscroller/qtscroller.pro \
          ../../../../Platform/Gui/Components/MainMenu/Build/MainMenu.pro \
          ../../../../Platform/Master/Components/DeviceControl/Build/DeviceControl.pro \
          ../../../../Platform/Master/Components/DeviceCommandProcessor/Build/DeviceCommandProcessor.pro \
          ../../../../Platform/Gui/Components/KeyBoard/Build/KeyBoard.pro \
          ../../../../Platform/Master/Components/GPIOManager/Build/GPIOManager.pro \
          ../../../../Platform/Master/Components/ImportExport/Build/ImportExport.pro \
          ../../../../Platform/Master/Components/ExternalProcessController/Build/ExternalProcessController.pro \
          ../../../../Platform/Master/Components/StateMachines/Build/StateMachines.pro \
          ../../../../Platform/Master/Components/ExportController/Build/ExportController.pro \
          ../../../../Platform/ServiceSW/Components/LogViewer/Build/LogViewer.pro \
          ../../../../Platform/ServiceSW/Components/Settings/Build/Settings.pro \
          ../../../Shared/Gui/Components/Images/Build/Images.pro \
          ../../../Shared/Master/Components/HimalayaDataContainer/Build/HimalayaDataContainer.pro \
          ../Components/TestCases/Build/TestCases.pro \
          ../Components/ServiceWidget/Build/ServiceWidget.pro \
          ../Components/Calibration/Build/Calibration.pro \
          ../Components/SystemTracking/Build/SystemTracking.pro \
          ../Components/LogViewerDialog/Build/LogViewerDialog.pro \
          ../Components/Diagnostics/Build/Diagnostics.pro \
          ../Components/DiagnosticsManufacturing/Build/DiagnosticsManufacturing.pro \
          ../Components/ServiceUpdates/Build/ServiceUpdates.pro \
          ../Components/Core/Build/Core.pro \
          ../Components/ServiceDeviceController/Build/ServiceDeviceController.pro \
          ../Components/ServiceMasterThreadController/Build/ServiceMasterThreadController.pro \
          ../Components/ServiceDataManager/Build/ServiceDataManager.pro \
          ../Components/Main/Build/Main.pro


CONFIG += ordered

#TRANSLATIONS = ../Components/Main/Build/TranslationsService/HimalayaService_en.pm \
#               ../Components/Main/Build/TranslationsService/HimalayaService_zh.pm

