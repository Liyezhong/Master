# build all components recursive

TEMPLATE = subdirs
SUBDIRS = ../../../Shared/Master/Components/HimalayaDataContainer/Build/HimalayaDataContainer.pro \
          ../Components/NetworkLayer/Build/NetworkLayer.pro \
          ../Components/Dashboard/Build/Dashboard.pro \
          ../Components/Core/Build/Core.pro \
          ../Components/Programs/Build/Programs.pro \
          ../Components/Reagents/Build/Reagents.pro \
          ../Components/Settings/Build/Settings.pro \
          ../Components/Users/Build/Users.pro \
          ../Components/Programs/Test/TestPrograms.pro \
          ../Components/Dashboard/Test/TestDashboard.pro \
          ../Components/Reagents/Test/TestReagents.pro

CONFIG += ordered
