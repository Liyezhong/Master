!include("TestDataManager.pri") {
    error("TestDataManager.pri not found")
}

TEMPLATE = subdirs
SUBDIRS = ../Containers/StainingPrograms/Test/TestDataProgramList.pro \
          ../Containers/StainingReagents/Test/TestDataReagentList.pro \
          ../Containers/ProgramSequence/Test/TestDataProgramSequenceList.pro \
          ../Containers/Stations/Test/TestDataStationList.pro \
          ../Containers/StationsGrid/Test/TestDataStationMatrix.pro \
		  
CONFIG += ordered


INCLUDEPATH += ../../../../HimalayaMain/Master/Components/
