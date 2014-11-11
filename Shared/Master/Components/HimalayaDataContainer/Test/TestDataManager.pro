!include("TestDataManager.pri") {
    error("TestDataManager.pri not found")
}

TEMPLATE = subdirs
#SUBDIRS  = ../Containers/ReagentStations/Test/TestDataStationList.pro
#SUBDIRS += ../Containers/Reagents/Test/ TestDataReagentList.pro
#SUBDIRS += ../Containers/ReagentGroups/Test/TestDataReagentGroupList.pro
#SUBDIRS += ../Containers/ProgramSettings/Test/TestPProgramSettings.pro
#SUBDIRS += ../Containers/Programs/Test/TestDataProgramList.pro
		  
CONFIG += ordered


INCLUDEPATH += ../../../../HimalayaMain/Master/Components/
