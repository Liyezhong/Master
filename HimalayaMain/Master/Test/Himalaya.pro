# build all components recursive

TEMPLATE = subdirs

SUBDIRS += ../../../Shared/Master/Test/Shared.pro \
#           ../Components/DBT/Test/DBT.pro \
#           ../Components/PhysicalDevice/Test/PhysicalDevice.pro \
           ../Components/Scheduler/Test/Scheduler.pro \
           ../Components/ExportController/Test/TestExportController.pro \
           ../Components/RMS/Test/TestRMSHandler.pro \
#          ../Components/BLG/Test/TestBLG.pro \
           ../Components/Verifier/Test/Verifier.pro \
           ../Components/ImportExport/Test/TestImportExportThreadController.pro

CONFIG += ordered
