# build all components recursive

TEMPLATE = subdirs

######## normal build process ########
#
# Make sure your Main.pro is in the last line to ensure correct linking!
#
SUBDIRS = ../Components/Calibration/Build/Calibration.pro \
          ../../../../Platform/Gui/Components/Application/Build/Application.pro \
          ../../../../Platform/Gui/Components/kineticscroller/qtscroller.pro \
          ../../../../Platform/Master/Components/ImportExport/Build/ImportExport.pro \
          ../../../../Platform/ServiceSW/Components/LogViewer/Build/LogViewer.pro \
          ../../../../Platform/ServiceSW/Components/Settings/Build/Settings.pro \
          ../Components/SystemTracking/Build/SystemTracking.pro \
          ../Components/LogViewer/Build/LogViewer.pro \
          ../Components/Diagnostics/Build/Diagnostics.pro \
#         ../Components/DiagnosticsManufacturing/Build/DiagnosticsManufacturing.pro \
          ../Components/ServiceUpdates/Build/ServiceUpdates.pro \
          ../Components/Core/Build/Core.pro \
          ../Components/ServiceDataManager/Build/ServiceDataManager.pro \
          ../Components/ServiceMasterThreadController/Build/ServiceMasterThreadController.pro \
#          ../Components/ServiceDeviceController/Build/ServiceDeviceController.pro \
          ../Components/SystemTracking/Test/TestSystemTracking.pro \
          ../Components/LogViewer/Test/TestLogViewer.pro \
          ../Components/Diagnostics/Test/TestDiagnostics.pro \
#          ../Components/DiagnosticsManufacturing/Test/TestDiagnosticsManufacturing.pro \
          ../Components/ServiceUpdates/Test/TestServiceUpdates.pro \
          ../Components/ImportExport/Test/TestServiceImportExportHandler.pro \
          ../Components/ServiceDataManager/Test/TestServiceDataManager.pro \
#          ../Components/ServiceDeviceController/Test/TestServiceDeviceController.pro

CONFIG += ordered

